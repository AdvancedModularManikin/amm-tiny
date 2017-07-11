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
	config.longSampleMode = kADC16_LongSampleCycle24;
	ADC16_Init(ADC1, &config);
	ADC16_EnableHardwareTrigger(ADC1, false);
	ADC16_DoAutoCalibration(ADC1); // can't do much with any errors

	channel_config.channelNumber = 13U;
	channel_config.enableInterruptOnConversionCompleted = false;
	channel_config.enableDifferentialConversion = false;
	return 0;
}

uint32_t raw_pressure = 0;
uint32_t unfiltered_pressure;

float ratio = 0.95;

void
polling_task(void *params)
{
	for (;;) {
		ADC16_SetChannelConfig(ADC1, 0U, &channel_config);
		while (0U == (kADC16_ChannelConversionDoneFlag &
				ADC16_GetChannelStatusFlags(ADC1, 0U)))
		{
		}
		unfiltered_pressure = ADC16_GetChannelConversionValue(ADC1, 0U);
		raw_pressure = raw_pressure * ratio + unfiltered_pressure * (1-ratio);

		PRINTF("ADC Value: %d\r\n", raw_pressure);
		PRINTF("ADC PSI  : %f\r\n", pressure::get_psi());
		vTaskDelay(20);
	}
	vTaskSuspend(NULL);
}

namespace pressure {
/*float
get_MPa(void)
{
	//convert to voltage
	uint32_t max = 4096;

	float top = 5;
	float bot = 1;
	float val_bot = -0.1;
	float val_top = 0.1;
	float frac = ((float) raw_pressure)/((float) max);
	// this is not actually volts, but it is what the sensor originally returned
	float volts = frac * top;
	float adj = (volts-bot)/(top-bot); //linear from 1 to 5

	return val_bot + adj*(val_top-val_bot); // multiply by 1MPa/1 for units
}
*/
float
get_psi(void)
{
	//convert to voltage
	uint32_t max = 4096;

	float top = 1.0377 ;
	float bot = 0.9303 ;
	float val_bot = 0.0;
	float val_top = 4.2;
	float frac = ((float) raw_pressure)/((float) max);
	// this is not actually volts, but it is what the sensor originally returned
	float volts = frac * 5.0;
	float adj = (volts-bot)/(top-bot);

	return val_bot + adj*(val_top-val_bot); // multiply by 1PSI/1 for units
}
}
