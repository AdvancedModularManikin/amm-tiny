/*
 * motorcontrol.cpp
 *
 *  Created on: Jul 7, 2017
 *      Author: gus
 */

#include "fsl_gpio.h"

#include "FreeRTOS.h"
#include "task.h"

#include "heartrate.h"

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
	//every ten seconds toggle the motor on/off
	for (;;) {
		motor_toggle();
		//vTaskDelay(2000); //two seconds
		vTaskDelay(led_delay_time > 0 ? led_delay_time : 3000);
	}
	vTaskSuspend(NULL);
}


