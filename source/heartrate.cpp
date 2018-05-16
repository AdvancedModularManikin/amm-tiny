/*
 * heartrate.cpp
 *
 *  Created on: Jul 6, 2017
 *      Author: gus
 */

#include "board.h"
//#include "pin_mux.h"
#include "clock_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "fsl_gpio.h"

#include "heartrate.h"
#include "ammdk-carrier/solenoid.h"

void
pin_hr_task(void *pvParameters)
{
	for (;;) {
		if (heart_delay_time > 0) {
			solenoid::toggle(solenoids[7]);
			GPIO_TogglePinsOutput(GPIOA, 1U<<24U);
		} else {
			solenoid::off(solenoids[7]);
			GPIO_ClearPinsOutput(GPIOA, 1U<<24U);
		}
		vTaskDelay(heart_delay_time > 0 ? heart_delay_time : 1000);
	}
	vTaskSuspend(NULL);
}

void
pin_br_task(void *pvParameters)
{
	for (;;) {
		if (breath_delay_time > 0) {
			solenoid::toggle(solenoids[6]);
		} else {
			solenoid::off(solenoids[6]);
		}
		vTaskDelay(breath_delay_time > 0 ? breath_delay_time : 1000);
	}
	vTaskSuspend(NULL);
}



