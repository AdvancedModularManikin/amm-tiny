#pragma once
#define CARRIER_GPIO_COUNT (7+8+1+1+1)
struct gpio_pin {
	GPIO_Type *base;
	PORT_Type *port;
	uint8_t pin_ix; // the mask is 1 << pin_ix
	clock_ip_name_t clock;
};

extern struct gpio_pin carrier_gpios[CARRIER_GPIO_COUNT];

void
carrier_gpio_init(void);
