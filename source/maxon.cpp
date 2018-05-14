#include "FreeRTOS.h"
#include "task.h"

#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_dac.h"
//#include "fsl_adc16.h"

#include "spi_proto.h"
#include "spi_proto_slave.h"
#include "ammdk-carrier/maxon.h"

#include <stdio.h>

uint32_t dacVal = 0, dacMax = 0xfff;

//0xfff = 3.3v, write function to convert float rpm to uint32 dac output
//3000 rpm
float rpm3000 = 3.143;
float rpm0 = 0.0;

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
void v24_on(void) {GPIO_SetPinsOutput(GPIOA, 1U<<7U);}
void v24_off(void) {GPIO_ClearPinsOutput(GPIOA, 1U<<7U);}

bool should_motor_run = false, should_24v_be_on = false;
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
		should_24v_be_on ? v24_on() : v24_off();
		should_motor_run ? maxon_on() : maxon_off();
		int in = GPIO_ReadPinInput(GPIOB, 2U);
		
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
