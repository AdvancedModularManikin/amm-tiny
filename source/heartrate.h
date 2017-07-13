/*
 * heartrate.h
 *
 *  Created on: Jul 6, 2017
 *      Author: gus
 */

#ifndef SOURCE_HEARTRATE_H_
#define SOURCE_HEARTRATE_H_


volatile TickType_t led_delay_time = 1000 / portTICK_PERIOD_MS;
volatile TickType_t breath_delay_time = 4000 / portTICK_PERIOD_MS;

void
pin_hr_task(void *pvParameters);


#endif /* SOURCE_HEARTRATE_H_ */
