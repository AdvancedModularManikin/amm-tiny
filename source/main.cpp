/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * This is template for main module created by New Kinetis SDK 2.x Project Wizard. Enjoy!
 **/

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

/* freescale drivers */
#include "fsl_dspi.h"
#include "fsl_dspi_freertos.h"
#include "fsl_adc16.h"
//#include "fsl_common.h"
//#include "fsl_gpio.h"
//#include "fsl_port.h"
//#include "fsl_ftm.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* entropic */
#include "spi_proto.h"
#include "spi_proto_slave.h"
#include "heartrate.h"
#include "solenoid.h"
#include "pressuresensor.h"
#include "button.h"
#include "flowsensor.h"
#include "spi_edma_task.h"

/* included just for tasks */
#include "sensirion.h"
#include "maxon.h"
#include <stdio.h>
#include "action.h"
#include "mule-1/air_tank.h"

/* Task priorities. */
#define hello_task_PRIORITY (configMAX_PRIORITIES - 2)

/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
static void hello_task(void *pvParameters) {
	int i = 0;
	for (;;) {
		char msg[32];
		snprintf(msg, 32, "Hello! %d ", i++);
		i %= 100;
		
		slave_send_message(spi_proto::p, (unsigned char*) msg, 32);
		vTaskDelay(100);
	}
	vTaskSuspend(NULL);
}

/*!
 * @brief Application entry point.
 */
int main(void) {
  /* Init board hardware. */
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();

  /* Add your code here */
  //TODO create spi_proto struct and mark it as unused
  //TODO setup flow interrupt

  gpio_pin_config_t digital_out = {kGPIO_DigitalOutput, (0)};
  GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, &digital_out);

  polling_init();
  BaseType_t ret;
  /* Create RTOS task */
  ret = xTaskCreate(pin_hr_task, "pin heartrate task", configMINIMAL_STACK_SIZE+100, NULL, hello_task_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  ret = xTaskCreate(pin_br_task, "pin breathrate task", configMINIMAL_STACK_SIZE+100, NULL, hello_task_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);

  ret = xTaskCreate(solenoid_gdb_mirror_task, "solenoid_gdb_mirror_task", configMINIMAL_STACK_SIZE+100, NULL, hello_task_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  
  //ret = xTaskCreate(hello_task, "Hello task", configMINIMAL_STACK_SIZE+1000, NULL, hello_task_PRIORITY, NULL);
  //assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  
  ret = xTaskCreate(spi_edma_task, "spi edma task", configMINIMAL_STACK_SIZE+200, NULL, hello_task_PRIORITY+1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  
  /*
  ret = xTaskCreate(sensirion_task, "Sensirion_task", configMINIMAL_STACK_SIZE + 124, NULL, hello_task_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  */
  //ret = xTaskCreate(carrier_board_test_task, "carrier_board_test_task", configMINIMAL_STACK_SIZE + 100, NULL, hello_task_PRIORITY, NULL);
  //assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);

  ret = xTaskCreate(carrier_pressure_task, "carrier_pressure_task", configMINIMAL_STACK_SIZE + 100, NULL, hello_task_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  
  ret = xTaskCreate(air_reservoir_control_task, "airtank control", configMINIMAL_STACK_SIZE + 100, NULL, hello_task_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  /*
  ret = xTaskCreate(button_task, "button task", configMINIMAL_STACK_SIZE + 1000, NULL, hello_task_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  ret = xTaskCreate(solenoid_task, "solenoid task", configMINIMAL_STACK_SIZE+1000, NULL, hello_task_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  //ret = xTaskCreate(motor_task, "motor task", configMINIMAL_STACK_SIZE, NULL, hello_task_PRIORITY, NULL);
  //assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  ret = xTaskCreate(polling_task, "polling task", configMINIMAL_STACK_SIZE + 1000, NULL, hello_task_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  */
  /*
  ret = xTaskCreate(flow_sensor_task, "flow sensor task", configMINIMAL_STACK_SIZE + 1000, NULL, hello_task_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  */
  
  ret = xTaskCreate(maxon_task, "Maxon task", configMINIMAL_STACK_SIZE + 100, NULL, hello_task_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  /*
  ret = xTaskCreate(action_task, "Action task", configMINIMAL_STACK_SIZE + 1000, NULL, hello_task_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  */
  
  /*
  ret = xTaskCreate(lung_task, "lung task", configMINIMAL_STACK_SIZE, NULL, hello_task_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  */
  
  vTaskStartScheduler();

  for(;;) { /* Infinite loop to avoid leaving the main function */
    __asm("NOP"); /* something to use as a breakpoint stop while looping */
  }
}



