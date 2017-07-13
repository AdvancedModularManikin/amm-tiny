/*
 * flowsensor.c
 *
 *  Created on: Jul 10, 2017
 *      Author: gus
 */

#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "FreeRTOS.h"
#include "task.h"

//steps to have a working flow sensor:
//initialize GPIO as input
//set up rise/fall interrupt
//make the interrupt add numbers
//set up recurring task to take the added-up count and put it somewhere useful

volatile uint32_t pulsecounts;
float flow_rate;

const float TICKS_PER_LITER = 1934.0;

#define GPIO GPIOB
#define PORT PORTB
#define PIN 4U
int
init(void)
{
	CLOCK_EnableClock(kCLOCK_PortB);
	port_pin_config_t flow_pin_settings = {0};
	flow_pin_settings.pullSelect = kPORT_PullDown;
	flow_pin_settings.mux = kPORT_MuxAsGpio;
	PORT_SetPinConfig(PORT, PIN, &flow_pin_settings);
	gpio_pin_config_t settings = {kGPIO_DigitalInput, 0};
	GPIO_PinInit(GPIO, PIN, &settings);
	EnableIRQ(PORTB_IRQn);
	PORT_SetPinInterruptConfig(PORT, PIN, kPORT_InterruptEitherEdge);
	NVIC_SetPriority(PORTB_IRQn, 5);
	pulsecounts = 0;
	//TODO blip B18

	GPIO_SetPinsOutput(GPIOB, 1 << 18U);
	GPIO_ClearPinsOutput(GPIOB, 1 << 18U);
	LED_GREEN_OFF();
	__enable_irq();
}

extern "C" {
void
PORTB_IRQHandler(void)
{
	//TODO blip B19
	GPIO_SetPinsOutput(GPIOB, 1 << 19U);

	LED_GREEN_ON();
	PORT_ClearPinsInterruptFlags(PORT, 1<<PIN);
	//uint32_t iflags = PORT_GetPinsInterruptFlags(PORTB);
	//if (iflags & (1<<4U)) {
	pulsecounts++;
	//}
	//FIXME currently clears all flags, is this the correct approach?
	//interrupt flags must be written with 1 to clear them

	GPIO_ClearPinsOutput(GPIOB, 1 << 19U);
}
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
		LED_GREEN_OFF();


		float ticks = pulsecounts;
		pulsecounts = 0;
		//TODO change to mL/min
		//have time, ticks, ticks/L, want L/s
		//flow_rate = (ticks / TICKS_PER_LITER) * 10; // L/s
		//to get mL/min, multiply L/s * mL/L * m/s = 1000/60
		flow_rate = (ticks / TICKS_PER_LITER) * 10.0 * 1000.0/60.0;

		PRINTF("flow_rate: %f\r\n", flow_rate);
		vTaskDelay(100);
	}

}
