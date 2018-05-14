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
#include "spi_proto.h"
#include "spi_proto_slave.h"
#include "solenoid.h"

#include "ammdk-carrier/solenoid.h"


void
solenoid::on(struct solenoid &s) {
	GPIO_SetPinsOutput(s.base, 1 << s.pin_ix);
}

void
solenoid::off(struct solenoid &s) {
	GPIO_ClearPinsOutput(s.base, 1 << s.pin_ix);
}

void
solenoid::toggle(struct solenoid &s) {
	GPIO_TogglePinsOutput(s.base, 1 << s.pin_ix);
}

void
carrier_board_test_task(void *params)
{
	//TODO init all solenoids
	
	for (;;) {
		//blink them
		for (int i = 0; i < 8; i++) {
			solenoid::toggle(solenoids[i]);
		}
		vTaskDelay(500);
	}
	
	vTaskSuspend(NULL);
}

volatile bool should_sol_n_be_on[SOLENOID_NUM] = {0};
volatile bool is_sol_n_manual[SOLENOID_NUM] = {0};
void
solenoid_gdb_mirror_task(void *params)
{
	for (;;) {
		for (int i = 0; i < SOLENOID_NUM; i++) {
			if (is_sol_n_manual[i])
				should_sol_n_be_on
					? solenoid::on(solenoids[i])
					: solenoid::off(solenoids[i]);
		}
		vTaskDelay(200);
	}
	
	vTaskSuspend(NULL);
}
