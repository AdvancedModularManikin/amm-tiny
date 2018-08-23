#define GPIO_NUM CARRIER_GPIO_COUNT

extern int gpio_bad_chunks;

void
gpio_handle_slave(struct gpio_cmd *cmd,
	struct gpio_pin *carrier_gpios,
	size_t gpio_num);

void
gpio_off(struct gpio_pin *p);
void
gpio_on(struct gpio_pin *p);
void
gpio_toggle(struct gpio_pin *p);
