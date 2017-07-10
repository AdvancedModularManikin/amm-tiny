/*
 * motorcontrol.h
 *
 *  Created on: Jul 7, 2017
 *      Author: gus
 */

#ifndef SOURCE_MOTORCONTROL_H_
#define SOURCE_MOTORCONTROL_H_

//C2 is enable/disable
//C5:3 is potentiometer control TODO write code, also handle which is which.

//note: the enable/disable is inverted, high is off

void
motor_on(void);

void
motor_off(void);

void
motor_task(void *params);


#endif /* SOURCE_MOTORCONTROL_H_ */
