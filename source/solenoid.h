/*
 * solenoid.h
 *
 *  Created on: Jul 6, 2017
 *      Author: gus
 */

#ifndef SOURCE_SOLENOID_H_
#define SOURCE_SOLENOID_H_

volatile bool tourniquet_on = false;
volatile bool hemorrhage_enabled = false;

void
carrier_board_test_task(void *params);
void
solenoid_gdb_mirror_task(void *params);

#endif /* SOURCE_SOLENOID_H_ */
