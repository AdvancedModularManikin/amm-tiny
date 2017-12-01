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
#include "spi_proto.h"
#include "spi_proto_slave.h"

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

#define MOTOR_SPEED_LEVELS 128

int motor_level;

void
motor_speed_up(void)
{
	//TODO
	//enable CS
	//U/~D to HI
	//pulse CLK, must last 25 ns
	//turn CLK off
	//disable CS
	motor_level++;
	if (motor_level >= MOTOR_SPEED_LEVELS) motor_level = MOTOR_SPEED_LEVELS-1;
}

void
motor_speed_down(void)
{
	//TODO
	//enable CS
	//U/~D to LOW
	//pulse CLK, must last 25 ns
	//turn CLK off
	//disable CS
	motor_level--;
	if (motor_level < 0) motor_level = 0;
}

void
motor_speed_init(void)
{
	for (int i = 0; i < MOTOR_SPEED_LEVELS;i++) motor_speed_down();
	motor_level = 0;
}
void
motor_task(void *params)
{
	motor_off();

	for (;;) {
		//TODO this needs to be debounced
		float psi = pressure::get_psi_1();

		if (spi_proto::spi_transactions > 0) {
			if (psi > 4.0) {
				motor_off();
			}
			if (psi < 2.5) {
				motor_on();
			}
		}
		vTaskDelay(20);
	}
	vTaskSuspend(NULL);
}


