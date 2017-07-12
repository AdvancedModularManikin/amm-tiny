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
	EnableIRQ(PORTB_IRQn);
	PORT_SetPinInterruptConfig(PORTB, 4U, kPORT_InterruptEitherEdge);
	NVIC_SetPriority(PORTB_IRQn, 6);
	pulsecounts = 0;
	//TODO blip B18

	GPIO_SetPinsOutput(GPIOB, 1 << 18U);
	GPIO_ClearPinsOutput(GPIOB, 1 << 18U);
}

void
PORTB_IRQHandler(void)
{
	//TODO blip B19
	GPIO_SetPinsOutput(GPIOB, 1 << 19U);

	PORT_ClearPinsInterruptFlags(PORTB, 1<<4U);
	//uint32_t iflags = PORT_GetPinsInterruptFlags(PORTB);
	//if (iflags & (1<<4U)) {
	pulsecounts++;
	//}
	//FIXME currently clears all flags, is this the correct approach?
	//interrupt flags must be written with 1 to clear them

	GPIO_ClearPinsOutput(GPIOB, 1 << 19U);

}

void
flow_sensor_task(void *params)
{
	init();
	//TODO every so often reset the tick counter and store the number of ticks in another value somewhere
	for (;;) {
		//TODO blip A25
		GPIO_SetPinsOutput(GPIOA, 1 << 25U);
		GPIO_ClearPinsOutput(GPIOA, 1 << 25U);


		float ticks = pulsecounts;
		pulsecounts = 0;
		//TODO change to mL/min
		//have time, ticks, ticks/L, want L/s
		//flow_rate = (ticks / TICKS_PER_LITER) * 10; // L/s
		//to get mL/min, multiply L/s * mL/L * m/s = 1000/60
		flow_rate = (ticks / TICKS_PER_LITER) * 10 * 1000/60;

		PRINTF("flow_rate: %f\r\n", flow_rate);
		vTaskDelay(100);
	}

}
