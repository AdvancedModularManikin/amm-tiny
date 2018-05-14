#include "stdint.h"
#include "stddef.h"
#include "fsl_gpio.h"
#include "ammdk-carrier/solenoid.h"
#include "valve.h"


//TODO relocate
unsigned int valve_bad_chunks = 0;


void valve_close(struct solenoid::solenoid &v);
void valve_open(struct solenoid::solenoid &v);
	

void
valve_handle_slave(struct valve_command cmd,
	struct solenoid::solenoid *valves,
	size_t valve_num)
{	
	//TODO
	//whole point of spi proto stuff was to be able to send deltas, so the command will be a valve id and a new state
	
	if (cmd.valve_id < valve_num) {
		if (cmd.valve_command == VALVE_CLOSED) {
			valve_close(valves[cmd.valve_id]);
		} else if (cmd.valve_command == VALVE_OPEN) {
			valve_open(valves[cmd.valve_id]);
		} else {
			valve_bad_chunks++;
		}
	}
}

#define POLARITY_ON_IS_OPEN 1
void
valve_close(struct solenoid::solenoid &v)
{
#if POLARITY_ON_IS_OPEN
	solenoid::off(v);
#else
	solenoid::on(v);
#endif
}
void
valve_open(struct solenoid::solenoid &v)
{
#if POLARITY_ON_IS_OPEN
	solenoid::on(v);
#else
	solenoid::off(v);
#endif
}
