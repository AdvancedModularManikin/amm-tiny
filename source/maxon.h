#pragma once
uint32_t dacVal;
bool should_motor_run, should_24v_be_on;

void
maxon_task(void *params);
