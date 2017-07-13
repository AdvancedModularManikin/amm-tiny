/*
 * solenoid.cpp
 *
 *  Created on: Jul 7, 2017
 *      Author: gus
 */
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"

#include "heartrate.h"
#include "spi_proto_slave.h"
#include "solenoid.h"


int
solenoids_toggle(void)
{
	//TODO toggle all solenoids

	GPIO_TogglePinsOutput(GPIOA, 1 << 25U);
	GPIO_TogglePinsOutput(GPIOB, 1 << 18U);
	GPIO_TogglePinsOutput(GPIOB, 1 << 19U);
	//GPIO_TogglePinsOutput(GPIOC, 1 << 10U);
	//GPIO_TogglePinsOutput(GPIOC, 1 << 11U);

	return 0;
}

void
hemorrhage_on(void) {GPIO_SetPinsOutput(GPIOC, 1 << 10U);}
void
hemorrhage_off(void) {GPIO_ClearPinsOutput(GPIOC, 1 << 10U);}
void
hemorrhage_toggle(void) {GPIO_TogglePinsOutput(GPIOC, 1 << 10U);}

void
swelling_on(void) {GPIO_SetPinsOutput(GPIOC, 1 << 11U);}
void
swelling_off(void) {GPIO_ClearPinsOutput(GPIOC, 1 << 11U);}
void
swelling_toggle(void) {GPIO_TogglePinsOutput(GPIOC, 1 << 11U);}

void
lungs_on(void) {GPIO_SetPinsOutput(GPIOB, 1 << 19U);}
void
lungs_off(void) {GPIO_ClearPinsOutput(GPIOB, 1 << 19U);}
void
lungs_toggle(void) {GPIO_TogglePinsOutput(GPIOB, 1 << 19U);}

//TODO make a task that waits on the first of any of a number of times
void
solenoid_task(void *params)
{

	hemorrhage_off();
	swelling_off();

	for (;;) {
		//solenoids_toggle();
		if (spi_proto::spi_transactions > 0) {
			hemorrhage_enabled ?
				tourniquet_on ? hemorrhage_off() : hemorrhage_toggle()
					: hemorrhage_off();
		}
		swelling_toggle();
		vTaskDelay(led_delay_time > 0 ? led_delay_time : 1000);
		//vTaskDelay(3000);
	}
	vTaskSuspend(NULL);
}

void
lung_task(void *params)
{
	lungs_off();

	for (;;) {
		lungs_toggle();
		vTaskDelay(breath_delay_time > 0 ? breath_delay_time : 4000);
	}
	vTaskSuspend(NULL);
}

