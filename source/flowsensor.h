/*
 * flowsensor.h
 *
 *  Created on: Jul 10, 2017
 *      Author: gus
 */

#ifndef SOURCE_FLOWSENSOR_H_
#define SOURCE_FLOWSENSOR_H_


void
init(void);

//not sure that this needs to be in a header
void
PORTB_IRQHandler(void);

void
flow_sensor_task(void *params);


#endif /* SOURCE_FLOWSENSOR_H_ */
