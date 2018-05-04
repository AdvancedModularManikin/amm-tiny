
#include "fsl_gpio.h"
#include "fsl_port.h"

#include "carrier_gpio.h"

struct gpio_pin carrier_gpios[CARRIER_GPIO_COUNT] = {
/*
1 PTB18	GPIO 1
2 PTB19	GPIO 2
3 PTB20	GPIO 3
4 	PTB21	GPIO 4
5 PTB22	GPIO 5
6 PTB23	GPIO 6
7 PTC0

	*/
	{GPIOB, PORTB, 18U, kCLOCK_PortB},
	{GPIOB, PORTB, 19U, kCLOCK_PortB},
	{GPIOB, PORTB, 20U, kCLOCK_PortB},
	{GPIOB, PORTB, 21U, kCLOCK_PortB},
	{GPIOB, PORTB, 22U, kCLOCK_PortB},
	{GPIOB, PORTB, 23U, kCLOCK_PortB},
	{GPIOC, PORTC, 00U, kCLOCK_PortC}
	
};

void carrier_gpio_init(void)
{
	gpio_pin_config_t in = {kGPIO_DigitalInput, 0};
	for (int i=0;i<CARRIER_GPIO_COUNT;i++) {
	    port_pin_config_t button_settings = {0};
	    button_settings.pullSelect = kPORT_PullUp;
	    button_settings.mux = kPORT_MuxAsGpio;

	    CLOCK_EnableClock(carrier_gpios[i].clock);
	    PORT_SetPinConfig(carrier_gpios[i].port, carrier_gpios[i].pin_ix, &button_settings);
		GPIO_PinInit(carrier_gpios[i].base, carrier_gpios[i].pin_ix, &in);
	}
}