/*
 * flowsensor.c
 *
 *  Created on: Jul 10, 2017
 *      Author: gus
 */

#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"

//steps to have a working flow sensor:
//initialize GPIO as input
//set up rise/fall interrupt
//make the interrupt add numbers
//set up recurring task to take the added-up count and put it somewhere useful

uint32_t pulsecounts;
float flow_rate;

const float TICKS_PER_LITER = 1934;

int
init(void)
{
	PORT_SetPinMux(PORTB, 4U, kPORT_MuxAsGpio);
	gpio_pin_config_t settings = {kGPIO_DigitalInput, 0};
	GPIO_PinInit(GPIOB, 4U, &settings);
	PORT_SetPinInterruptConfig(PORTB, 4U, kPORT_InterruptEitherEdge);
	NVIC_SetPriority(PORTB_IRQn, 5);
	pulsecounts = 0;
}

void
PORTB_IRQHandler(void)
{
	uint32_t iflags = PORT_GetPinsInterruptFlags(PORTB);
	if (iflags & (1<<4U)) {
		pulsecounts++;
	}
	//FIXME currently clears all flags, is this the correct approach?
	//interrupt flags must be written with 1 to clear them
	PORT_ClearPinsInterruptFlags(PORTB, ~0U);
}

void
flow_sensor_task(void *params)
{
	init();
	//TODO every so often reset the tick counter and store the number of ticks in another value somewhere
	for (;;) {

		float ticks = pulsecounts;
		pulsecounts = 0;
		//have time, ticks, ticks/L, want L/s
		flow_rate = (ticks / TICKS_PER_LITER) * 10;
		PRINTF("flow_rate: %f\r\n", flow_rate);
		vTaskDelay(100);
	}

}
