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

/* solenoids on ammdk generic app board mule 1
1 PTD4
2 PTD5
3 PTD6
4 PTD7
5 PTA10
6 PTA11
7 PTA12
8 PTA13
*/

//extern "C" {
struct solenoid::solenoid solenoids[8] = {
	{.base = GPIOD, .pin_ix = 4},
	{.base = GPIOD, .pin_ix = 5},
	{.base = GPIOD, .pin_ix = 6},
	{.base = GPIOD, .pin_ix = 7},
	{.base = GPIOA, .pin_ix = 10},
	{.base = GPIOA, .pin_ix = 11},
	{.base = GPIOA, .pin_ix = 12},
	{.base = GPIOA, .pin_ix = 13}
};
//}

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
