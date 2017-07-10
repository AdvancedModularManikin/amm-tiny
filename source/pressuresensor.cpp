/*
 * pressuresensor.cpp
 *
 *  Created on: Jul 10, 2017
 *      Author: gus
 */

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "fsl_adc16.h"
#include "fsl_debug_console.h"

#include "pressuresensor.h"


//changing to polling. TODO figure out what the problem with the interrupts was
//possible problems: lack of proper pin configuration, issue with priority
//(i.e. what NVIC_SetPriority changes)

adc16_config_t config;
adc16_channel_config_t channel_config;

int
polling_init(void) {

	ADC16_GetDefaultConfig(&config);
	ADC16_Init(ADC1, &config);
	ADC16_EnableHardwareTrigger(ADC1, false);
	ADC16_DoAutoCalibration(ADC1); // can't do much with any errors

	channel_config.channelNumber = 13U;
	channel_config.enableInterruptOnConversionCompleted = false;
	channel_config.enableDifferentialConversion = false;
	return 0;
}

uint32_t raw_pressure;

void
polling_task(void *params)
{
	for (;;) {
		ADC16_SetChannelConfig(ADC1, 0U, &channel_config);
		while (0U == (kADC16_ChannelConversionDoneFlag &
				ADC16_GetChannelStatusFlags(ADC1, 0U)))
		{
		}
		raw_pressure = ADC16_GetChannelConversionValue(ADC1, 0U);
		PRINTF("ADC Value: %d\r\n", raw_pressure);
		PRINTF("ADC Value: %f\r\n", pressure::get_MPa());
		vTaskDelay(20);
	}
	vTaskSuspend(NULL);
}

namespace pressure {
float
get_MPa(void)
{
	//convert to voltage
		uint32_t max = 4096;

		float top = 5;
		float bot = 1;
		float frac = ((float) raw_pressure)/((float) max);
		// this is not actually volts, but it is what the sensor originally returned
		float volts = frac * top;
		float adj = (volts-1)/4; //linear from 1 to 5
		return frac;//adj; // multiply by 1MPa/1 for units
}
float
get_psi(void)
{
	return 145.038*get_MPa();
}
}
