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

#include <stdio.h>
#include "spi_proto.h"
#include "spi_proto_slave.h"

//#include "fsl_adc16.h"

//steps to have a working flow sensor:
//initialize GPIO as input
//set up rise/fall interrupt
//make the interrupt add numbers
//set up recurring task to take the added-up count and put it somewhere useful

volatile uint32_t pulsecounts;
float flow_rate;

const float TICKS_PER_MILLILITER = 22.0;
uint32_t total_pulses = 0;

#define CLOCK kCLOCK_PortC
#define GPIO GPIOC
#define PORT PORTC
#define PIN 9U
void
init(void)
{
	CLOCK_EnableClock(CLOCK);
	port_pin_config_t flow_pin_settings = {0};
	flow_pin_settings.pullSelect = kPORT_PullUp;
	flow_pin_settings.mux = kPORT_MuxAsGpio;
	PORT_SetPinConfig(PORT, PIN, &flow_pin_settings);
	gpio_pin_config_t settings = {kGPIO_DigitalInput, 0};
	GPIO_PinInit(GPIO, PIN, &settings);
	EnableIRQ(PORTC_IRQn);
	PORT_SetPinInterruptConfig(PORT, PIN, kPORT_InterruptEitherEdge);
	NVIC_SetPriority(PORTC_IRQn, 4);
	pulsecounts = 0;

	//TODO almost certain this doesn't matter
	__enable_irq();
}

extern "C" {
void
PORTC_IRQHandler(void)
{
	//TODO re-enable the flag checking (to prepare for multiple pin interrupts) and then TODO test it
	PORT_ClearPinsInterruptFlags(PORT, 1U<<PIN);
	//uint32_t iflags = PORT_GetPinsInterruptFlags(PORTB);
	//if (iflags & (1<<4U)) {
	pulsecounts++;
	//}
	//FIXME currently clears all flags, is this the correct approach?
	//interrupt flags must be written with 1 to clear them
}
}

void
flow_sensor_task(void *params)
{
	init();
	//TODO every so often reset the tick counter and store the number of ticks in another value somewhere
	for (;;) {
		float ticks = pulsecounts;
		total_pulses += pulsecounts;
		pulsecounts = 0;
		//TODO change to mL/min
		//have time, ticks, ticks/L, want L/s
		//flow_rate = (ticks / TICKS_PER_LITER) * 10; // mL/s
		//to get mL/s, multiply by 10
		flow_rate = (ticks / TICKS_PER_MILLILITER) * 10.0;
		char msg[32];
		snprintf(msg, 32, "flow_rate: %d uL/s", (int)(flow_rate*1000));
		
		slave_send_message(spi_proto::p, (unsigned char*) msg, 32);

		//PRINTF("flow_rate: %f\r\n", flow_rate);
		vTaskDelay(100);
	}

}
