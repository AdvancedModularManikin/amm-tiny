#include <stdint.h>
#include <stddef.h>
#include "motor.h"
#include "spi_remote.h"

uint16_t motor_bad_chunks;
void
motor_handle_slave(struct motor_command *cmd,
	struct motor *carrier_motors,
	size_t motor_num)
{
	if (cmd->motor_id >= motor_num) {motor_bad_chunks++;}
	
	switch (cmd->cmd.ty) {
		case MOTOR_CMD_OFF:
		motor_off(&carrier_motors[cmd->motor_id]);
		break;
		case MOTOR_CMD_ON:
		motor_on(&carrier_motors[cmd->motor_id]);
		break;
		case MOTOR_CMD_SETSPEED:
		motor_set_speed(&carrier_motors[cmd->motor_id], cmd->cmd.param);
		break;
	}
}
