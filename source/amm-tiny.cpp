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
#include "fsl_adc16.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* entropic */
#include "spi_proto.h"
#include "spi_proto_slave.h"
#include "spi_chunks_slave.h"
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
#include "ammdk-carrier/maxon.h"
#include "ammdk-carrier/dac.h"

/* Task priorities. */
#define max_PRIORITY (configMAX_PRIORITIES - 1)

void
ammtinycb(struct spi_packet *p)
{
	spi_chunk_overall(p->msg, SPI_MSG_PAYLOAD_LEN);
}

/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
static void puppet_task(void *pvParameters) {
	int i = 0;
	for (;;) {
		char msg[32];
		snprintf(msg, 32, "Hello! %d ", i++);
		i %= 100;
		
		slave_send_message(spi_proto::p, (unsigned char*) msg, 32);
		vTaskDelay(pdMS_TO_TICKS(100));
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

  gpio_pin_config_t digital_out = {kGPIO_DigitalOutput, (0)};
  GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, &digital_out);

  maxon_init();
  dac_init();
  polling_init();
  BaseType_t ret;
  /* Create RTOS task */
  ret = xTaskCreate(solenoid_gdb_mirror_task, "solenoid_gdb_mirror_task", configMINIMAL_STACK_SIZE+100, NULL, max_PRIORITY-1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  
  ret = xTaskCreate(spi_edma_task, "spi edma task", configMINIMAL_STACK_SIZE+200, (void*)ammtinycb, max_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  
  /*
  ret = xTaskCreate(sensirion_task, "Sensirion_task", configMINIMAL_STACK_SIZE + 124, NULL, max_PRIORITY-1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  */

  ret = xTaskCreate(carrier_pressure_task, "carrier_pressure_task", configMINIMAL_STACK_SIZE + 100, NULL, max_PRIORITY-1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  
  /*
  ret = xTaskCreate(air_reservoir_control_task, "airtank control", configMINIMAL_STACK_SIZE + 100, NULL, max_PRIORITY-1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  */
  /*
  ret = xTaskCreate(solenoid_task, "solenoid task", configMINIMAL_STACK_SIZE+1000, NULL, max_PRIORITY-1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  //ret = xTaskCreate(motor_task, "motor task", configMINIMAL_STACK_SIZE, NULL, max_PRIORITY-1, NULL);
  //assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  */
  
  //ret = xTaskCreate(maxon_task, "Maxon task", configMINIMAL_STACK_SIZE + 100, NULL, max_PRIORITY-1, NULL);
  //assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  
  vTaskStartScheduler();

  for(;;) { /* Infinite loop to avoid leaving the main function */
    __asm("NOP"); /* something to use as a breakpoint stop while looping */
  }
}



