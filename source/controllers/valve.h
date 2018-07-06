//valve commands are just the states, implicitly the command is "change state", if more commands are added this must be changed

//valve states
//TODO look up syntax for naming this and specifying storage size explicitly
enum valve_state {
	VALVE_CLOSED = 0,
	VALVE_OPEN = 1
};

//TODO check whether attribute packed is necessary here
struct valve_command {
	uint8_t valve_id;
	uint8_t valve_command;
};
#define CHUNK_LEN_VALVE sizeof(struct valve_command)
#define VALVE_NUM SOLENOID_NUM

void
valve_handle_slave(struct valve_command *cmd,
	struct solenoid::solenoid *valves,
	size_t valve_num);
