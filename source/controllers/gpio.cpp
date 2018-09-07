#include "fsl_gpio.h"
#include "../ammdk-carrier/carrier_gpio.h"
#include "gpio.h"

#include "spi_remote.h"
#include "spi_proto_lib/spi_chunk_defines.h"
#include "spi_chunks_slave.h"

int gpio_bad_chunks = 0;

void
gpio_handle_slave(struct gpio_cmd *cmd,
	struct gpio_pin *carrier_gpios,
	size_t gpio_num)
{
	if (cmd->id >= gpio_num) {gpio_bad_chunks++; return;}
	
	int ret;
	uint8_t buf[5]; // TODO assert CHUNK_LEN_GPIO_S2M == 5
	
	switch (cmd->cmd) {
	case OP_SET:
		switch(cmd->val) {
		case GPIO_ON:
			gpio_on(&carrier_gpios[cmd->id]);
			goto set_resp;
		case GPIO_TOGGLE:
			gpio_toggle(&carrier_gpios[cmd->id]);
			goto set_resp;
		case GPIO_OFF:
			gpio_off(&carrier_gpios[cmd->id]);
			goto set_resp;
		default:
			{gpio_bad_chunks++; return;}
		}
	case OP_GET:
	//cmd val meaningless here
		ret = gpio_read(&carrier_gpios[cmd->id]);
		buf[0] = 0;
		buf[1] = CHUNK_TYPE_GPIO;
		buf[2] = cmd->id;
		buf[3] = OP_GET;
		buf[4] = ret;
		send_chunk(buf, 5);
		return;
	default:
		return;
	}
	
	//TODO this should be five I think
	set_resp:
	uint8_t buf_set[] = {0, CHUNK_TYPE_GPIO, cmd->id, OP_SET};
	send_chunk(buf_set, 4);
	return;
}

int
gpio_read(struct gpio_pin *p)
{
	return GPIO_ReadPinInput(p->base, 1U << p->pin_ix);
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
