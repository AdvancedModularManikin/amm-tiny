#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "maxon.h"
#include "fsl_adc16.h"
#include "pressuresensor.h"
#include "ammdk-carrier/solenoid.h"

struct pid_ctl {
	float p;
	float i;
	float d;
	float target;
	
	float isum; // current value
	float last;
	float last_diff;
};

float
pi_supply(struct pid_ctl *p, float reading)
{
	float diff = reading - p->last;
	p->last = reading;
	p->last_diff = diff;
	float oset = p->target - reading;
	
	p->isum += oset * p->i;
	
	return p->isum + p->p*oset + p->d*diff;
}

struct pid_ctl pid;

uint16_t stall_val = 0x100;

float ret;
uint32_t val;
void
air_reservoir_control_task(void *params)
{
	solenoid::off(solenoids[0]);
	solenoid::on(solenoids[1]);
	
	pid.p = 24;
	pid.i = 1.0/1024;
	pid.d = 1.0/16;
	pid.isum = 0;
	pid.target = 5;
	
	should_24v_be_on = 1;
	should_motor_run = 1;
	
	//voltage divider
	float r1 = 1200;
	float r2 = 2200;
	float voldiv = r2/(r1+r2);
	
	for (;;) {
		uint32_t adcRead = carrier_sensors[0].raw_pressure;
		float psi = ((float)adcRead)*(3.0/10280.0*16.0) - 15.0/8.0;
		
		ret = pi_supply(&pid, psi);
		
		//TODO convert back to 0-2^12 range for DAC
		val = (uint32_t) (ret*1000.0);
		should_motor_run = stall_val < val;
		dacVal = val > 0xfff ? 0xfff : val;
		
		vTaskDelay(50);
	}
	vTaskSuspend(NULL);
}

