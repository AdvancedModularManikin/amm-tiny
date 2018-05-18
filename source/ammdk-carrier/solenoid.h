#pragma once
#define SOLENOID_NUM 8
namespace solenoid {
struct solenoid {
	GPIO_Type *base;
	uint8_t pin_ix; // the mask is 1 << pin_ix
	//int clock; //TODO other stuff that's needed
};

//it's possible that extend/contract would be better, but on/off will do
	void
	on(struct solenoid &s);
	void
	off(struct solenoid &s);
	void
	toggle(struct solenoid &s);
}

int
solenoids_toggle(void);
extern struct solenoid::solenoid solenoids[SOLENOID_NUM];
