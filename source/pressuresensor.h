/*
 * pressuresensor.h
 *
 *  Created on: Jul 10, 2017
 *      Author: gus
 */

#ifndef SOURCE_PRESSURESENSOR_H_
#define SOURCE_PRESSURESENSOR_H_

int
polling_init(void);
void
polling_task(void *params);

namespace pressure {
float
get_MPa(void);
float
get_psi(void);
}


#endif /* SOURCE_PRESSURESENSOR_H_ */
