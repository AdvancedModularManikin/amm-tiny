#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "maxon.h"
#include "fsl_adc16.h"
#include "pressuresensor.h"

struct pi_ctl {
	float p;
	float i;
	float target;
	
	float isum; // current value
};

float
pi_supply(struct pi_ctl *p, float reading)
{
	float oset = p->target - reading;
	
	p->isum += oset * p->i;
	
	return p->isum + p->p*oset;
}

struct pi_ctl pid;

void
air_reservoir_control_task(void *params)
{
	pid.p = 0.25;
	pid.i = 0.01;
	pid.isum = 3; //not really based on anything
	pid.target = 5;
	
	should_24v_be_on = 1;
	should_motor_run = 1;
	
	for (;;) {
		uint32_t presh = carrier_sensors[0].raw_pressure;
		
		//convert to PSI. Assume linearity
		
		float psi = ((float)(presh * 15))/(1<<16);
		
		float ret = pi_supply(&pid, psi);
		
		//TODO convert back to 0-2^12 range for DAC
		uint32_t val = (uint32_t) (ret*0x200);
		dacVal = val;
		
		vTaskDelay(100);
	}
	vTaskSuspend(NULL);
}

