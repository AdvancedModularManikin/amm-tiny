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

#include "spi_proto.h"
#include "spi_proto_slave.h"

#include <stdio.h>


//changing to polling.
//possible problems: lack of proper pin configuration, issue with priority
//(i.e. what NVIC_SetPriority changes)
//the problem was lack of extern "C" TODO go back to interrupt based


#define NUMBER_OF_PRESSURE_SENSORS 2
//extern "C" {
struct pressure_sensor sensors[NUMBER_OF_PRESSURE_SENSORS] = {
		{
				.channel_config = {
						.channelNumber = 13U, // B7
						.enableInterruptOnConversionCompleted = false,
						.enableDifferentialConversion = false
				}
		}, {
				.channel_config = {
						.channelNumber = 12U, // B6
						.enableInterruptOnConversionCompleted = false,
						.enableDifferentialConversion = false
				}
		}
};
//};

int
polling_init(void) {
	adc16_config_t config;
	
	ADC16_GetDefaultConfig(&config);
	config.longSampleMode = kADC16_LongSampleCycle24;
	ADC16_Init(ADC1, &config);
	ADC16_EnableHardwareTrigger(ADC1, false);
	ADC16_DoAutoCalibration(ADC1); // can't do much with any errors

	return 0;
}

uint32_t raw_pressure = 0;
uint32_t unfiltered_pressure;

float ratio = 0.95;

void
polling_task(void *params)
{
	for (;;) {
		for (int i = 0; i < NUMBER_OF_PRESSURE_SENSORS; i++) {
			ADC16_SetChannelConfig(ADC1, 0U, &sensors[i].channel_config);
			while (0U == (kADC16_ChannelConversionDoneFlag &
					ADC16_GetChannelStatusFlags(ADC1, 0U)))
			{
			}
			sensors[i].unfiltered_pressure = ADC16_GetChannelConversionValue(ADC1, 0U);
			sensors[i].raw_pressure = sensors[i].raw_pressure * ratio + sensors[i].unfiltered_pressure * (1-ratio);
		}

		//PRINTF("ADC Value: %d \t\t %d\r\n", sensors[0].raw_pressure, sensors[1].raw_pressure);
		//PRINTF("ADC Value: %d\r\n", sensors[1].raw_pressure);
		//PRINTF("ADC PSI  : %f\t%f\r\n", pressure::get_psi_1(),pressure::get_psi_2());
		vTaskDelay(20);
	}
	vTaskSuspend(NULL);
}

namespace pressure {
float
get_psi_1(void)
{
	//convert to voltage
	uint32_t max = 4096;

	float top = 1.0377 ;
	float bot = 0.9303 ;
	float val_bot = 0.0;
	float val_top = 4.2;
	float frac = ((float) sensors[0].raw_pressure)/((float) max);
	// this is not actually volts, but it is what the sensor originally returned
	float volts = frac * 5.0;
	float adj = (volts-bot)/(top-bot);

	return val_bot + adj*(val_top-val_bot); // multiply by 1PSI/1 for units
}

float // TODO change to bool that checks >= 15.0
get_psi_2(void)
{
	//convert to voltage
	uint32_t max = 4096;
	//pressure sensor, 0-5v (we divide it 0-3.3v) range is 2.2-16.7PSI on PB6

	float top = 5.0 ;
	float bot = 0.0 ;
	float val_bot = 2.2;
	float val_top = 16.7;
	float frac = ((float) sensors[1].raw_pressure)/((float) max);
	// this is not actually volts, but it is what the sensor originally returned
	float volts = frac * 5.0;
	float adj = (volts-bot)/(top-bot);

	return val_bot + adj*(val_top-val_bot); // multiply by 1PSI/1 for units
}
}

//carrier board pressure sensors
/*
Pressure1 ADC1 SE11 B5
Pressure2 ADC1 SE12 B6
Pressure3 ADC1 SE13 B7
Pressure4 ADC1 SE14 B10
*/
struct pressure_sensor carrier_sensors[4] = {
		{
				.channel_config = {
						.channelNumber = 11U,
						.enableInterruptOnConversionCompleted = false,
						.enableDifferentialConversion = false
				}
		}, {
				.channel_config = {
						.channelNumber = 12U,
						.enableInterruptOnConversionCompleted = false,
						.enableDifferentialConversion = false
				}
		}, {
				.channel_config = {
						.channelNumber = 13U,
						.enableInterruptOnConversionCompleted = false,
						.enableDifferentialConversion = false
				}
		}, {
				.channel_config = {
						.channelNumber = 14U,
						.enableInterruptOnConversionCompleted = false,
						.enableDifferentialConversion = false
				}
		}
};


void
carrier_pressure_task(void *params)
{
	//TODO possibly prepare gpios, but I didn't do this with the last ones
	
	for (;;) {
		for (int i = 0; i < 4; i++) {
			ADC16_SetChannelConfig(ADC1, 0U, &carrier_sensors[i].channel_config);
			while (0U == (kADC16_ChannelConversionDoneFlag &
					ADC16_GetChannelStatusFlags(ADC1, 0U)))
			{
			}
			carrier_sensors[i].unfiltered_pressure = ADC16_GetChannelConversionValue(ADC1, 0U);
			carrier_sensors[i].raw_pressure = carrier_sensors[i].raw_pressure * ratio + carrier_sensors[i].unfiltered_pressure * (1-ratio);
		}
		
		char msg[32];
		snprintf(msg, 32, "ADC Value: %lu \t%lu \t%lu \t%lu\r\n",
			carrier_sensors[0].raw_pressure,
			carrier_sensors[1].raw_pressure,
			carrier_sensors[2].raw_pressure,
			carrier_sensors[3].raw_pressure);
		slave_send_message(spi_proto::p, (unsigned char*) msg, 32);
		
		vTaskDelay(20);
	}
	vTaskSuspend(NULL);
}
