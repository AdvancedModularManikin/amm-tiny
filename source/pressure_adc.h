//
// Created by Peter O'Hanley on 8/9/17.
//

#ifndef PROJECT_PRESSURE_ADC_H
#define PROJECT_PRESSURE_ADC_H
struct pressure_sensor {
	//adc16_config_t config; // only need one per ADC
	adc16_channel_config_t channel_config;
	uint32_t raw_pressure = 0;
	uint32_t unfiltered_pressure;
};
namespace pressure {

//TODO I think ADCs can only do one channel at once, which means the relevant
//info needs to be stored separately if we want to do multiple on the same ADC
	struct sensor {
		//TODO, at least needs a reference to the register where the result is
		//stored and a way to get the value out
		//TODO store pointer to conversion function in here so that it is possible to
		//have different kinds of pressure sensors
		ADC_Type *adc;
		IRQn_Type adc_int;
		uint32_t channel_group; // in software mode, this is always 0
		adc16_config_t config;
		adc16_channel_config_t channelconfig;
		uint16_t last_value;
		float (*transform)(uint16_t);
	};
}


void
start_pressure_subsystem(void);
void
pressure_logging_task(void *params);

#endif //PROJECT_PRESSURE_ADC_H
