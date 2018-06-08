enum motor_command_ty {
	MOTOR_CMD_OFF = 0, MOTOR_CMD_ON = 1, MOTOR_CMD_SETSPEED = 2
};

struct motor_command_raw {
	enum motor_command_ty ty;
	uint16_t param;
};

enum motor_state_ty {
	MOTOR_ST_OFF = 0, MOTOR_ST_ON = 1
};

struct motor_state {
	enum motor_state_ty ty;
	uint16_t param;
};

struct motor_command {
	uint8_t motor_id;
	struct motor_command_raw cmd;
};
#define CHUNK_LEN_MOTOR sizeof(struct motor_command)

struct motor {
	int motor_id; // TODO all contents
};

void
motor_handle_slave(struct motor_command *cmd,
	struct motor *carrier_motors,
	size_t motor_num);

void
motor_set_speed(struct motor *m, uint16_t spd);

void
motor_on(struct motor *m);

void
motor_off(struct motor *m);
