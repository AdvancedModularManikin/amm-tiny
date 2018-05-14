#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

#include "fsl_gpio.h"

#include "ammdk-carrier/carrier_gpio.h"

#include "spi_proto.h"
#include "spi_proto_slave.h"

struct action_button {
	struct gpio_pin *pin;
	const char *action;
	
	int debounce;
};

#define ACTION_NUM 3
struct action_button actions[ACTION_NUM] = {
	{&carrier_gpios[4], "LEG_HEMORRHAGE", 0},
	{&carrier_gpios[5], "PROPER_TOURNIQUET", 0},
	{&carrier_gpios[6], "ARM_L_SUCC_140MG", 0},
};

int action_err = 0;
int lastread[ACTION_NUM]= {0};
void
action_task(void *params)
{
	carrier_gpio_init();
	
	for (;;) {
		for (int i = 0;i<ACTION_NUM;i++) {
			if (lastread[i] = !GPIO_ReadPinInput(actions[i].pin->base, actions[i].pin->pin_ix)) {
				if (!actions[i].debounce) {
					actions[i].debounce = 1;
					char msg[32];
					snprintf(msg, 32, "ACTION: %s", actions[i].action);
					action_err |= slave_send_message(spi_proto::p, (unsigned char*) msg, 32);
				}
			}
		}
		
		vTaskDelay(10/portTICK_PERIOD_MS);
	}
	vTaskSuspend(NULL);
}
