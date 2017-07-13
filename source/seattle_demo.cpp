/*
 * seattle_demo.cpp
 *
 *  Created on: Jul 13, 2017
 *      Author: gus
 */

#include "fsl_gpio.h"
#include "fsl_port.h"

#include "FreeRTOS.h"
#include "task.h"

#include "solenoid.h"
#include "spi_proto_slave.h"

void
seattle_init(void)
{
	//TODO initialize switch GPIO B5
	CLOCK_EnableClock(kCLOCK_PortB);
	port_pin_config_t start_switch_settings = {0};
	start_switch_settings.pullSelect = kPORT_PullUp;
	start_switch_settings.mux = kPORT_MuxAsGpio;
	PORT_SetPinConfig(PORTB, 5U, &start_switch_settings);
	gpio_pin_config_t gpioconfig = {kGPIO_DigitalInput, 0};
	GPIO_PinInit(GPIOB, 5U, &gpioconfig);
}

void
seattle_task(void *params)
{
	seattle_init();

	for (;;) {
		//monitor switch
		//if switch pressed, low = pressed
		if (GPIO_ReadPinInput(GPIOB, 5U) == 0){
			//TODO send start message

			//TODO send hemorrhage message
			unsigned char buf[4];
			buf[2] = 1; // start
			buf[1] = 1; // hemorrhage
			spi_proto::slave_send_message(spi_proto::p, buf, 4);
			//TODO start bleeding (in time with heartrate
			hemorrhage_enabled = true;
			break;
		}
		vTaskDelay(200);
	}
	//wait for tourniquet happens in slave_get_message, so this is done
	vTaskSuspend(NULL);
}
