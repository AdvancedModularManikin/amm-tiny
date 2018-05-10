//valve commands are just the states, implicitly the command is "change state", if more commands are added this must be changed

//valve states
//TODO look up syntax for naming this and specifying storage size explicitly
enum valve_state {
	VALVE_CLOSED = 0,
	VALVE_OPEN = 1
};
typedef int *GPIO;
struct valve {
	GPIO gpio;
	enum valve_state cur_state;
	//TODO polarity: is it open on 0 or 1?
};

//TODO check whether attribute packed is necessary here
struct valve_command {
	uint8_t valve_id;
	uint8_t valve_command;
};
//TODO calculate from struct, then assert that it is 2
#define VALVE_PAYLOAD_LEN 2
#define VALVE_CHUNK_LEN (VALVE_PAYLOAD_LEN + 1 + 1)
//TODO move
#define VALVE_NUM 3

void
valve_handle_slave(struct valve_command cmd,
	struct solenoid::solenoid *valves,
	size_t valve_num);
