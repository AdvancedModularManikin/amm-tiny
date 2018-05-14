/*
 * solenoid.h
 *
 *  Created on: Jul 6, 2017
 *      Author: gus
 */

#ifndef SOURCE_SOLENOID_H_
#define SOURCE_SOLENOID_H_

//TODO should this be split off into a gpiopin struct? wait until other things are using it
//on and off functions just trigger pins. possibly instead store pin info in the struct?

volatile bool tourniquet_on = false;
volatile bool hemorrhage_enabled = false;

void
solenoid_task(void *params);
void
carrier_board_test_task(void *params);
void
lung_task(void *params);

#endif /* SOURCE_SOLENOID_H_ */
