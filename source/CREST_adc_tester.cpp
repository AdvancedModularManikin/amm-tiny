/*
 * The Clear BSD License
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "board.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"
#include "fsl_port.h"
#include "fsl_adc16.h"

#include "clock_config.h"
/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* entropic */
#include "spi_proto.h"
#include "spi_proto_slave.h"
#include "spi_chunks_slave.h"
#include "solenoid.h"
#include "pressuresensor.h"
#include "spi_edma_task.h"

/* Task priorities. */
#define max_PRIORITY (configMAX_PRIORITIES - 1)

//relocated pin_mux.c here to ensure it's not doing anything extraneous and get rid of coupling between executables in this codebase. non-main files shouldn't do hardware pokes unless they're only used by one executable
#include "fsl_device_registers.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "pin_mux.h"
#include "board.h"


void BOARD_InitPins(void)
{
    /* Initialize UART0 pins below */
    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Affects PORTB_PCR16 register */
    PORT_SetPinMux(PORTB, 16u, kPORT_MuxAlt3);
    /* Affects PORTB_PCR17 register */
    PORT_SetPinMux(PORTB, 17u, kPORT_MuxAlt3);

    CLOCK_EnableClock(kCLOCK_PortD);
    /* SPI on D0-D3 */
    PORT_SetPinMux(PORTD, 0U, kPORT_MuxAlt2);
    PORT_SetPinMux(PORTD, 1U, kPORT_MuxAlt2);
    PORT_SetPinMux(PORTD, 2U, kPORT_MuxAlt2);
    PORT_SetPinMux(PORTD, 3U, kPORT_MuxAlt2);

    //I2C0 pins. If you remove this the spi stops working
    CLOCK_EnableClock(kCLOCK_PortC);
    port_pin_config_t i2c1_pin_config = {0};
    i2c1_pin_config.mux = kPORT_MuxAlt2;
    i2c1_pin_config.openDrainEnable = kPORT_OpenDrainEnable;
    PORT_SetPinConfig(PORTC, 10U, &i2c1_pin_config); // I2C0_SCL
    PORT_SetPinConfig(PORTC, 11U, &i2c1_pin_config); // I2C0_SDA
}

void ammtinycb(struct spi_packet *p)
{
}

void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 800000; ++i) {
        __asm("NOP"); /* delay */
    }
}

struct adctest {
	ADC_Type *base;
	uint8_t adcix;
	unsigned int channelgroup;
	unsigned int channelNumber;
	int16_t last_result;
	adc16_channel_config_t adc16ChannelConfigStruct;
	bool isDiff;
  adc16_channel_mux_mode_t muxMode;
};
//TODO spit ADC over serial port

#define ADC_LINE(IX, NUMBER) {ADC ## IX , IX , 0 , NUMBER , 0 , {0}, 0, kADC16_ChannelMuxA}
#define ADC_LINE_MUX(IX, NUMBER, MUXLETTER) {ADC ## IX, IX, 0, NUMBER , 0, {0}, 0, kADC16_ChannelMux ## MUXLETTER}

//The labels below are for the AMMDK 2.0
struct adctest adcs[] = {
  ADC_LINE(0, 19), //ADC0_DM0    U11 (also ADC1_DM3. ADC0_DM0 accessible as channel 0b10011)
  //TODO ADC1_DM0, ADC0_DM3
  ADC_LINE(0,  3), //ADC0_DP3    U10 (also ADC1_DP0. with DIFF=0 measures ADC0_DP3 ADC0 channel 0b00011)
  //TODO CMP0_IN0
  //TODO CMP2_IN0
  //TODO CMP2_IN1
  //TODO CMP3_IN1
  //ADC_LINE(0,  8), //ADC0_SE8  U7_MOTOR_PWM (also ADC1_SE8)
  //ADC_LINE(0,  9), //ADC0_SE9  U7_MOTOR_ENABLE (also ADC1_SE9)
  //ADC_LINE(0, 10), //ADC0_SE10 J6_8
  //ADC_LINE(0, 11), //ADC0_SE11 J6_6
  ADC_LINE(0, 12), //ADC0_SE12 U7_MOTOR_STATUS
  //ADC_LINE(0, 13), //ADC0_SE13 U7_MOTOR_IO
  ADC_LINE(0, 14), //ADC0_SE14 J9_7
  //ADC_LINE(0, 15), //ADC0_SE15 J6_7
  //ADC_LINE(0, 16), //ADC0_SE16 J16_36 N/A (also CMP1_IN2, ADC0_SE21)
  //ADC_LINE(0, 17), //ADC0_SE17 J4_2
  ADC_LINE(0, 18), //ADC0_SE18 U8
  //ADC_LINE(0, 23), //ADC0_SE23 J17_35 N/A
  ADC_LINE(1, 10), //ADC1_SE10 U7_MOTOR_SPEED_OUT
  ADC_LINE(1, 11), //ADC1_SE11 U3
  ADC_LINE(1, 12), //ADC1_SE12 U4
  ADC_LINE(1, 13), //ADC1_SE13 U5
  ADC_LINE(1, 14), //ADC1_SE14 U6
  ADC_LINE(1, 15), //ADC1_SE15 J22_2
  ADC_LINE(1, 16), //ADC1_SE16 J22_3 (also ADC0_SE22)
  //ADC_LINE(1, 17), //ADC1_SE17 J6_3
  ADC_LINE(1, 18), //ADC1_SE18 U9
  //ADC_LINE(1, 23), //ADC1_SE23 U7_MOTOR_SPEED_IN
  //ADC_LINE_MUX(0, 4, B), //ADC0_SE4b J17_14 N/A
  //ADC_LINE_MUX(0, 6, B), //ADC0_SE6b J4_7
  //ADC_LINE_MUX(0, 7, B), //ADC0_SE7b J4_6
  //ADC_LINE_MUX(1, 4, A), //ADC1_SE4a J17_23 N/A
  //ADC_LINE_MUX(1, 5, A), //ADC1_SE5a J17_24 SPI MOSI (so N/A)
  ADC_LINE_MUX(1, 5, B), //ADC1_SE5b J8_7
  //ADC_LINE_MUX(1, 6, A), //ADC1_SE6a J17_21 SPI SCLK (so N/A)
  ADC_LINE_MUX(1, 6, B), //ADC1_SE6b J8_4 (I2C1_SCL)
  //ADC_LINE_MUX(1, 7, A), //ADC1_SE7a J17_26 SPI MISO (so N/A)
  ADC_LINE_MUX(1, 7, B), //ADC1_SE7b J8_1 (I2C1_SDA)
};

adc16_config_t adc16ConfigStructs[2];

#define ADC_NUM (sizeof(adcs)/sizeof(adcs[0]))

/* NOTE we don't init any pins because ALT0 (the default pin mux selection) is
 * already analog.
 */
void
adc_init(void)
{

	//adc16_config_t adc16ConfigStructs[2];
	/*
	 * adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceVref;
	 * adc16ConfigStruct.clockSource = kADC16_ClockSourceAsynchronousClock;
	 * adc16ConfigStruct.enableAsynchronousClock = true;
	 * adc16ConfigStruct.clockDivider = kADC16_ClockDivider8;
	 * adc16ConfigStruct.resolution = kADC16_ResolutionSE12Bit;
	 * adc16ConfigStruct.longSampleMode = kADC16_LongSampleDisabled;
	 * adc16ConfigStruct.enableHighSpeed = false;
	 * adc16ConfigStruct.enableLowPower = false;
	 * adc16ConfigStruct.enableContinuousConversion = false;
	 */
	ADC16_GetDefaultConfig(&adc16ConfigStructs[0]);
	ADC16_GetDefaultConfig(&adc16ConfigStructs[1]);
#ifdef BOARD_ADC_USE_ALT_VREF
	adc16ConfigStruct[0].referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
	adc16ConfigStruct[1].referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
#endif
	ADC16_Init(ADC0, &adc16ConfigStructs[0]);
	ADC16_Init(ADC1, &adc16ConfigStructs[1]);
	ADC16_EnableHardwareTrigger(ADC0, false); /* Make sure the software trigger is used. */
	ADC16_EnableHardwareTrigger(ADC1, false); /* Make sure the software trigger is used. */
#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
	if (kStatus_Success == ADC16_DoAutoCalibration(ADC0)) {
		//PRINTF("ADC16_DoAutoCalibration() 0 Done.\r\n");
	} else {
		//PRINTF("ADC16_DoAutoCalibration() 0 Failed.\r\n");
	}
	if (kStatus_Success == ADC16_DoAutoCalibration(ADC1)) {
		//PRINTF("ADC16_DoAutoCalibration() 1 Done.\r\n");
	} else {
		//PRINTF("ADC16_DoAutoCalibration() 1 Failed.\r\n");
	}
#endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */


	for (unsigned int i = 0; i < ADC_NUM; i++) {
    //adcs[i].adc16_channel_config_t adc16ChannelConfigStruct;

    adcs[i].adc16ChannelConfigStruct.channelNumber = adcs[i].channelNumber;
    adcs[i].adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adcs[i].adc16ChannelConfigStruct.enableDifferentialConversion = adcs[i].isDiff;
#endif /* FSL_FEATURE_ADC16_HAS_DIFF_MODE */
	}
}
void
adc_step(void)
{
	//read one value from each ADC
	// foreach adc, also store last value
	for (unsigned int i = 0; i < ADC_NUM; i++) {
        /*
         When in software trigger mode, each conversion would be launched once calling the "ADC16_ChannelConfigure()"
         function, which works like writing a conversion command and executing it. For another channel's conversion,
         just to change the "channelNumber" field in channel's configuration structure, and call the
         "ADC16_ChannelConfigure() again.
        */
    ADC16_SetChannelMuxMode(adcs[i].base, adcs[i].muxMode);
        ADC16_SetChannelConfig(adcs[i].base, adcs[i].channelgroup, &adcs[i].adc16ChannelConfigStruct);
        while (0U == (kADC16_ChannelConversionDoneFlag &
                      ADC16_GetChannelStatusFlags(adcs[i].base, adcs[i].channelgroup)))
        {
        }
		adcs[i].last_result = ADC16_GetChannelConversionValue(adcs[i].base, adcs[i].channelgroup);
    //PRINTF("ADC %02d Value: %d\r\n", i, adcs[i].last_result);
    }
  for (unsigned int i = 0; i < ADC_NUM; i++) {
        unsigned char buf[10];
        memset(buf, 0, 10);
        buf[0] = adcs[i].adcix;
        buf[1] = adcs[i].channelNumber;
        buf[2] = adcs[i].muxMode;
        buf[3] = adcs[i].last_result >> 8u;
        buf[4] = adcs[i].last_result & 0xff;
        buf[5] = i;
        int ret;
        do {
          ret = slave_send_message(spi_proto::p, (unsigned char*) buf, 10);
        } while (ret);
    }
}

volatile uint16_t last_results[ADC_NUM];

void
adc_test_task(void *param)
{
  for (;;) {
		adc_step();
    for (unsigned int i = 0; i < ADC_NUM; i++) {
      last_results[i] = adcs[i].last_result;
    }
		//delay();
    vTaskDelay(100);
	}
}
/*!
 * @brief Main function
 */
int main(void)
{
	/* Board pin, clock, debug console init */
  BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();

	adc_init();
  BaseType_t ret;

  ret = xTaskCreate(adc_test_task, "adc_test_task", configMINIMAL_STACK_SIZE + 100, NULL, max_PRIORITY-1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  ret = xTaskCreate(spi_edma_task, "spi edma task", configMINIMAL_STACK_SIZE+200, (void*)ammtinycb, max_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);

  vTaskStartScheduler();
	for(;;) { /* Infinite loop to avoid leaving the main function */
    __asm("NOP"); /* something to use as a breakpoint stop while looping */
  }
}
