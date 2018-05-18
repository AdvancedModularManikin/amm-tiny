/*
 * flowsensor.h
 *
 *  Created on: Jul 10, 2017
 *      Author: gus
 */

#ifndef SOURCE_FLOWSENSOR_H_
#define SOURCE_FLOWSENSOR_H_

uint32_t total_pulses;
float total_flow;

void
init(void);

//not sure that this needs to be in a header
void
PORTB_IRQHandler(void);

void
flow_sensor_task(void *params);


#endif /* SOURCE_FLOWSENSOR_H_ */
