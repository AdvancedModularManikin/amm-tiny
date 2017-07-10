/*
 * motorcontrol.cpp
 *
 *  Created on: Jul 7, 2017
 *      Author: gus
 */

#include "fsl_gpio.h"
#include "fsl_adc16.h"

#include "FreeRTOS.h"
#include "task.h"

#include "pressuresensor.h"

void
motor_on(void)
{
	//write C2 low
	GPIO_ClearPinsOutput(GPIOC, 1<<2U);
}

void
motor_off(void)
{
	//write C2 high
	GPIO_SetPinsOutput(GPIOC, 1<<2U);
}

void
motor_toggle(void)
{
	GPIO_TogglePinsOutput(GPIOC, 1<<2U);
}

void
motor_task(void *params)
{
	motor_off();

	for (;;) {
		float psi = pressure::get_psi();

		if (psi > 4.0) {
			motor_off();
		}
		if (psi < 2.5) {
			motor_on();
		}
		vTaskDelay(20);
	}
	vTaskSuspend(NULL);
}


