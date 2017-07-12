/*
 * button.cpp
 *
 *  Created on: Jul 11, 2017
 *      Author: gus
 */

#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "board.h"

#include "FreeRTOS.h"
#include "task.h"

#include "spi_proto_slave.h"
#include "solenoid.h"

unsigned char last_message;
void
button_task(void *params)
{
	const TickType_t delay_time = 100 / portTICK_PERIOD_MS;
	uint32_t sw2_down, sw3_down;
	//initialize button pins
	gpio_pin_config_t sw2_config = { kGPIO_DigitalInput, 0 };
	gpio_pin_config_t sw3_config = { kGPIO_DigitalInput, 0 };

	GPIO_PinInit(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN, &sw3_config);
	//strangely no SW2 defines in board.h
	GPIO_PinInit(GPIOD, 11U, &sw2_config);

	vTaskDelay(500);
	PRINTF("Buttons ready\r\n");
	for (;;) {
		if (!spi_proto::ready) continue;
		//check buttons
		sw2_down = GPIO_ReadPinInput(GPIOD, 11);
		sw3_down = GPIO_ReadPinInput(BOARD_SW3_GPIO,BOARD_SW3_GPIO_PIN);
		//TODO (undone) do stuff to debounce presses and ensure we only send the change event

		if (!sw2_down) tourniquet_on = true;

		//notify SPI thread
		unsigned char message = sw2_down | (sw3_down<<1);
		if (message != last_message) {
			slave_send_message(spi_proto::p, &message, 1);
			last_message = message;
		}

		PRINTF("sending button: %02x\r\n", message);

		vTaskDelay(delay_time);
	}
	vTaskSuspend(NULL);
}
