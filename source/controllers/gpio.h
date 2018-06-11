#define GPIO_NUM CARRIER_GPIO_COUNT

enum gpio_command_ty {
	GPIO_C_OFF = 0, GPIO_C_ON = 1, GPIO_C_TOGGLE = 2
};

enum gpio_state {
	GPIO_S_OFF = 0, GPIO_S_ON = 1
};

struct gpio_command {
	uint8_t gpio_id;
	uint8_t gpio_command;
};
#define CHUNK_LEN_GPIO sizeof(struct gpio_command)

struct gpio_response {
	uint8_t gpio_id;
	uint8_t gpio_val;
};

extern int gpio_bad_chunks;

void
gpio_handle_slave(struct gpio_command *cmd,
	struct gpio_pin *carrier_gpios,
	size_t gpio_num);

void
gpio_off(struct gpio_pin *p);
void
gpio_on(struct gpio_pin *p);
void
gpio_toggle(struct gpio_pin *p);
