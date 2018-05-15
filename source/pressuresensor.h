/*
 * pressuresensor.h
 *
 *  Created on: Jul 10, 2017
 *      Author: gus
 */

#ifndef SOURCE_PRESSURESENSOR_H_
#define SOURCE_PRESSURESENSOR_H_

//extern "C" {
struct pressure_sensor {
	//adc16_config_t config; // only need one per ADC
	adc16_channel_config_t channel_config;
	uint32_t raw_pressure;
	uint32_t unfiltered_pressure;
};
//}

extern struct pressure_sensor carrier_sensors[4];

int
polling_init(void);
void
polling_task(void *params);
void
carrier_pressure_task(void *params);

namespace pressure {
float
get_psi_1(void);
float
get_psi_2(void);
}


#endif /* SOURCE_PRESSURESENSOR_H_ */
