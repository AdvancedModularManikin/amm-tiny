#include "fsl_gpio.h"
#include "solenoid.h"

/* solenoids on ammdk generic app board mule 1
1 PTD4
2 PTD5
3 PTD6
4 PTD7
5 PTA10
6 PTA11
7 PTA12
8 PTA13
*/

//TODO we are keeping the solenoids around, but converting them to use PWM for the drop-and-hold behavior
//extern "C" {
struct solenoid::solenoid solenoids[SOLENOID_NUM] = {
	{.base = GPIOD, .pin_ix = 4},
	{.base = GPIOD, .pin_ix = 5},
	{.base = GPIOD, .pin_ix = 6},
	{.base = GPIOD, .pin_ix = 7},
	{.base = GPIOA, .pin_ix = 10},
	{.base = GPIOA, .pin_ix = 11},
	{.base = GPIOA, .pin_ix = 12},
	{.base = GPIOA, .pin_ix = 13}
};
//}
