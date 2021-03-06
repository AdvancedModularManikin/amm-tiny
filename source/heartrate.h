/*
 * heartrate.h
 *
 *  Created on: Jul 6, 2017
 *      Author: gus
 */

#ifndef SOURCE_HEARTRATE_H_
#define SOURCE_HEARTRATE_H_

volatile unsigned char heart_rate = 60;
volatile unsigned char breath_rate = 15;

volatile TickType_t heart_delay_time = 1000 / portTICK_PERIOD_MS;
volatile TickType_t breath_delay_time = 2000 / portTICK_PERIOD_MS;

void
pin_hr_task(void *pvParameters);
void
pin_br_task(void *pvParameters);


#endif /* SOURCE_HEARTRATE_H_ */
