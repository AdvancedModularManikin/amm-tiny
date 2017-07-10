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

#include "heartrate.h"

void
pin_hr_task(void *pvParameters)
{
	for (;;) {
		if (led_delay_time > 0) {
			LED_GREEN_TOGGLE();
		}
		vTaskDelay(led_delay_time > 0 ? led_delay_time : 1000);
	}
	vTaskSuspend(NULL);
}



