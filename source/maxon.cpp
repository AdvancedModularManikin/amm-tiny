#include "FreeRTOS.h"
#include "task.h"

#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_dac.h"
//#include "fsl_adc16.h"

#include "spi_proto.h"
#include "spi_proto_slave.h"

#include <stdio.h>

uint32_t dacVal = 0, dacMax = 0xfff;

//0xfff = 3.3v, write function to convert float rpm to uint32 dac output
//3000 rpm
float rpm3000 = 3.143;
float rpm0 = 0.0;

//adc16_channel_config_t adc16ChannelConfigStruct;

void
maxon_init(void)
{
	//B1 is motor enable (GPIO)
	//B4 is motor speed feedback (ADC)
	//B2 is motor status feedback (GPIO)
	//DAC0 is motor DAC speed control
	
	//setup B1 as GPIO out
    port_pin_config_t motor_enable_settings = {0};
    motor_enable_settings.pullSelect = kPORT_PullDown;
    motor_enable_settings.mux = kPORT_MuxAsGpio;
    CLOCK_EnableClock(kCLOCK_PortB);
    PORT_SetPinConfig(PORTB, 1U, &motor_enable_settings);
    gpio_pin_config_t digital_out = {kGPIO_DigitalOutput, (0)};
    GPIO_PinInit(GPIOB, 1U, &digital_out);
	//setup B2 as GPIO in
    port_pin_config_t motor_status_settings = {0};
    motor_status_settings.pullSelect = kPORT_PullUp;
    motor_status_settings.mux = kPORT_MuxAsGpio;
    CLOCK_EnableClock(kCLOCK_PortB);
    PORT_SetPinConfig(PORTB, 2U, &motor_status_settings);
    gpio_pin_config_t digital_in = {kGPIO_DigitalInput, (0)};
    GPIO_PinInit(GPIOB, 2U, &digital_in);
	
	//set up DAC0 as demo dac
	dac_config_t dac0_config;
	DAC_GetDefaultConfig(&dac0_config);
	DAC_Init(DAC0, &dac0_config);
	DAC_SetBufferReadPointer(DAC0, 0U);
	/*
	//enable the adc on B4 (ADC1_SE10)
    adc16_config_t adc16ConfigStruct;
    ADC16_GetDefaultConfig(&adc16ConfigStruct);
    ADC16_Init(ADC1, &adc16ConfigStruct);
    ADC16_EnableHardwareTrigger(ADC1, false);
    ADC16_DoAutoCalibration(ADC1);

    adc16ChannelConfigStruct.channelNumber = 10U;
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
    adc16ChannelConfigStruct.enableDifferentialConversion = false;
	*/
}


float
adc_scale(uint16_t adcval)
{
	//convert to rpm
	/*
		0--->adcval--->2^16
		0---> adcV ---> 3.3
		0---> rpmV ---> 3.143
		0---> rpm  ---> 3000
	*/
	
	float adcV = (((float) adcval) * 3.3)/((float) (1U<<12));
	float rpmV = adcV / 3.143;
	float rpm = rpmV * 3000.0;
	
	//((float)adcval)/4096.0* 3.3 /3.143 * 3000.0

	return rpm;
}

void maxon_on(void) {GPIO_SetPinsOutput(GPIOB, 1U<<1U);}
void maxon_off(void) {GPIO_ClearPinsOutput(GPIOB, 1U<<1U);}

bool should_motor_run = false;
uint16_t motorReading;
float scaleRes;
void
maxon_task(void *params)
{
	maxon_init();
	//write motor enable high
	GPIO_ClearPinsOutput(GPIOB, 1U<<1U);
	
	//every so often print status of motor ready
	for (;;) {
		should_motor_run ? maxon_on() : maxon_off();
		int in = GPIO_ReadPinInput(GPIOB, 1U<<2U);
		
		char msg[32];
		snprintf(msg, 32, "motor_ready: %d uL/s", in);
		
		slave_send_message(spi_proto::p, (unsigned char*) msg, 32);
		
		DAC_SetBufferValue(DAC0, 0U, dacVal > dacMax ? dacMax : dacVal);
		/*
        ADC16_SetChannelConfig(ADC1, 0U, &adc16ChannelConfigStruct);
        while (0U == (kADC16_ChannelConversionDoneFlag &
                      ADC16_GetChannelStatusFlags(ADC1, 0U)))
        {
        }
		*/
		//motorReading = ADC16_GetChannelConversionValue(ADC1, 0U);
		scaleRes = adc_scale(motorReading);
		
		snprintf(msg, 32, "motor reading: %d", (int) scaleRes);
		slave_send_message(spi_proto::p, (unsigned char*) msg, 32);
		
		vTaskDelay(1000);
	}
	vTaskSuspend(NULL);
}
