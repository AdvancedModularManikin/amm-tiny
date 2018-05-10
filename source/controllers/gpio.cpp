#include "fsl_gpio.h"
#include "../carrier_gpio.h"
#include "gpio.h"

int gpio_bad_chunks = 0;

void
gpio_handle_slave(struct gpio_command cmd,
	struct gpio_pin *carrier_gpios,
	size_t gpio_num)
{
	if (cmd.gpio_id >= gpio_num) {gpio_bad_chunks++; return;}
	
	switch (cmd.gpio_command) {
		case GPIO_C_OFF:
		gpio_off(&carrier_gpios[cmd.gpio_id]);
		break;
		case GPIO_C_ON:
		gpio_on(&carrier_gpios[cmd.gpio_id]);
		break;
		case GPIO_C_TOGGLE:
		gpio_toggle(&carrier_gpios[cmd.gpio_id]);
		break;
	}
}

void
gpio_off(struct gpio_pin *p)
{
	GPIO_ClearPinsOutput(p->base, 1U << p->pin_ix);
}
void
gpio_on(struct gpio_pin *p)
{
	GPIO_SetPinsOutput(p->base, 1U << p->pin_ix);
}
void
gpio_toggle(struct gpio_pin *p)
{
	GPIO_TogglePinsOutput(p->base, 1U << p->pin_ix);
}
