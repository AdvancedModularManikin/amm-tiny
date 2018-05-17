//
// Created by Peter O'Hanley on 8/9/17.
//

#include "FreeRTOS.h"
#include "task.h"
#include "fsl_adc16.h"
#include "fsl_debug_console.h"
#include "pressure_adc.h"

//steps to have a working pressure sensor
//initialize pin in ADC mode, continuous processing
//have a function that can be called to convert the ADC value to a value representing pressure
namespace pressure {

	float
	simple_transform(uint16_t val)
	{
		float top = 3.3;
		float v = val;
		float bittop = ((uint32_t) 1)<<16;

		return top * (v/bittop);
	}

	int
	setup(struct sensor &s)
	{
		//EnableIRQ(s.adc_int); //TODO find a way to enforce that the appropriate interrupt handler actually exists

		ADC16_Init(s.adc, &s.config); // there is a default config.
		ADC16_EnableHardwareTrigger(s.adc, false);
		return ADC16_DoAutoCalibration(s.adc); // can fail, but not really any action we can take if it does
		//ADC16_GetChannelConversionValue(s.adc, s.channel_group);
		//the bit that enables the interrupt on channel completino only lasts for one conversion. It must be re-enabled each time.

		//we return the value of the auto calibration, maybe it's useful
	}

	int
	setup_and_start(struct sensor &s)
	{
		int ret = setup(s);
		//TODO put this into ISR also
		ADC16_SetChannelConfig(s.adc, s.channel_group, &s.channelconfig);
		return ret;
	}

//TODO make this the whole interrupt
//#define adc_isr(name, s) ADC16_GetChannelConversionValue(s.adc, s.channel_group);


	float current_pressure_value(struct sensor &s)
	{
		//TODO this is not the correct transformation
		return s.transform(s.last_value);
	}

//pressure ports are B7:4
//B7 = ADC1_SE13
//B6 = ADC1_SE12
//B5 = ADC1_SE11
//B4 = ADC1_SE10
//these are AD10 to AD13

#define PRESSURE_SENSOR_HANDLER ADC1_IRQHandler

//all of these need to have interrupt on completion enabled as well as the same ADC
	int next_pressure = 0;
#define PRESSURE_COUNT 4
	unsigned int channels[4] = {10, 11, 12, 13};
	struct sensor pressures[PRESSURE_COUNT];

	void
	init(void)
	{

		for (int i = 0; i < 4; i++) {
			//setup all of the pressures at once
			pressures[i].adc = ADC1;
			pressures[i].adc_int = ADC1_IRQn;
			pressures[i].channel_group = 0U;
			ADC16_GetDefaultConfig(&pressures[i].config);
			//TODO rest of config settings, if any need to be changed
			pressures[i].channelconfig = {channels[i], true, false};
			pressures[i].last_value = 0;
			pressures[i].transform = &simple_transform;
		}
	}
}
//adc_isr(PRESSURE_SENSOR_HANDLER, pressure)

//TODO proper way to handle these multiple adcs is probably one loop like this
//for each ADC that is in use, so the `struct sensor` will be different,
//probably top-level stuff with a sublist of channels that are active
extern "C" { // required for linking with asm that defines handlers
void
PRESSURE_SENSOR_HANDLER(void) {
	using namespace pressure;
	pressures[next_pressure].last_value = ADC16_GetChannelConversionValue(
			pressures[next_pressure].adc,
			pressures[next_pressure].channel_group);

#if 0
	//increment current channel
	next_pressure++;
	next_pressure %= PRESSURE_COUNT;
	//next conversion
	//TODO currently this doesn't resume. We want to trigger a semaphore which awakens a task which waits and then
	// restarts the cycle
	if ((next_pressure+1) < PRESSURE_COUNT)
		ADC16_SetChannelConfig(pressures[next_pressure].adc,
			pressures[next_pressure].channel_group,
			&pressures[next_pressure].channelconfig);
#endif
}
}

void
start_pressure_subsystem(void)
{
	using namespace pressure;
	init();

	setup(pressures[1]);
	setup(pressures[2]);
	setup(pressures[3]);
	setup_and_start(pressures[0]);
}

void
pressure_logging_task(void *params)
{
	for (;;) {
		PRINTF("%04x\n", pressure::pressures[0].last_value);
		vTaskDelay(1000);
	}
	vTaskSuspend(NULL);
}
