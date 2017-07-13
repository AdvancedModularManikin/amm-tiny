/*
 * solenoid.h
 *
 *  Created on: Jul 6, 2017
 *      Author: gus
 */

#ifndef SOURCE_SOLENOID_H_
#define SOURCE_SOLENOID_H_

//solenoids TODO improve to an actual struct
#if 0
struct solenoid {
	void (*on)(void);
	void (*off)(void);
};

//on and off functions just trigger pins. possibly instead store pin info in the struct?

struct solenoid {
	int port;
	int pin;
	int clock; //TODO other stuff that's needed
};
#endif

volatile bool tourniquet_on = false;
volatile bool hemorrhage_enabled = false;

int
solenoids_toggle(void);

void
solenoid_task(void *params);

#endif /* SOURCE_SOLENOID_H_ */
