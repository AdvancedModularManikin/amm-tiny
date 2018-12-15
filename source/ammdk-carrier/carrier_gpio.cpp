
#include "fsl_gpio.h"
#include "fsl_port.h"

#include "carrier_gpio.h"

struct gpio_pin carrier_gpios[CARRIER_GPIO_COUNT] = {
#define DECL(letter, number) {GPIO ## letter , PORT ## letter , number , kCLOCK_Port ## letter}

	{GPIOB, PORTB, 18U, kCLOCK_PortB},
	{GPIOB, PORTB, 19U, kCLOCK_PortB},
	{GPIOB, PORTB, 20U, kCLOCK_PortB},
	{GPIOB, PORTB, 21U, kCLOCK_PortB},
	{GPIOB, PORTB, 22U, kCLOCK_PortB}, // J21_1
	{GPIOB, PORTB, 23U, kCLOCK_PortB}, // J21_2
	{GPIOC, PORTC, 00U, kCLOCK_PortC}, //J21_3
	DECL(D, 4), // J4 solenoids begin
	DECL(D, 5), // J5
	DECL(D, 6), // J6
	DECL(D, 7), // J7
	DECL(A, 10), // J8
	DECL(A, 11), // J9
	DECL(A, 12), // J10
	DECL(A, 13), // J11 solenoids end
	DECL(A, 7), // 24V rail
	DECL(B, 1), // motor enable
	DECL(A, 24), // an LED on the main board. TODO different definitions for main and shield stuff that's combined into one
#undef DECL
};

void carrier_gpio_init(void)
{
	gpio_pin_config_t in = {kGPIO_DigitalOutput, 0};
	for (int i=0;i<CARRIER_GPIO_COUNT;i++) {
	    port_pin_config_t button_settings = {0};
	    button_settings.pullSelect = kPORT_PullUp;
	    button_settings.mux = kPORT_MuxAsGpio;

	    CLOCK_EnableClock(carrier_gpios[i].clock);
	    PORT_SetPinConfig(carrier_gpios[i].port, carrier_gpios[i].pin_ix, &button_settings);
		GPIO_PinInit(carrier_gpios[i].base, carrier_gpios[i].pin_ix, &in);
	}
}
