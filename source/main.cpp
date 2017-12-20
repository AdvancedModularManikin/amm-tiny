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
#include "motorcontrol.h"
#include "solenoid.h"
#include "pressuresensor.h"
#include "button.h"
#include "flowsensor.h"
#include "seattle_demo.h"

/* Task priorities. */
#define hello_task_PRIORITY (configMAX_PRIORITIES - 1)

/* SPI */
#define EXAMPLE_DSPI_SLAVE_BASE (SPI0_BASE)
#define EXAMPLE_DSPI_SLAVE_BASEADDR ((SPI_Type *)EXAMPLE_DSPI_SLAVE_BASE)
#define EXAMPLE_SPI_SLAVE_IRQ SPI0_IRQn

#define TRANSFER_BAUDRATE (8388608U) /*! Transfer baudrate - 500k */

/* SPI data structures and functions */
uint8_t slaveReceiveBuffer[TRANSFER_SIZE] = {0};
uint8_t slaveSendBuffer[TRANSFER_SIZE] = {0};

dspi_slave_handle_t g_s_handle;
SemaphoreHandle_t dspi_sem;

typedef struct _callback_message_t
{
    status_t async_status;
    SemaphoreHandle_t sem;
} callback_message_t;

void DSPI_SlaveUserCallback(SPI_Type *base, dspi_slave_handle_t *handle, status_t status, void *userData)
{
    callback_message_t *cb_msg = (callback_message_t *)userData;
    BaseType_t reschedule;

    cb_msg->async_status = status;
    xSemaphoreGiveFromISR(cb_msg->sem, &reschedule);
    portYIELD_FROM_ISR(reschedule);
}

/*
dspi_slave_config_t slaveConfig = {
	{
		8, //bitsPerFrame
		kDSPI_ClockPolarityActiveHigh, //cpol
		kDSPI_ClockPhaseFirstEdge //cpha
	},
	false, //enableContinuousSCK
	false, //enableRxFifoOverWrite
	false, //enableModifiedTimingFormat
	kDSPI_SckToSin0Clock //samplePoint
};
*/
//*
extern "C" {
dspi_slave_config_t slaveConfig = {
	.whichCtar = kDSPI_Ctar0,
	.ctarConfig = {
		.bitsPerFrame = 8,
		.cpol = kDSPI_ClockPolarityActiveHigh,
		.cpha = kDSPI_ClockPhaseFirstEdge,
	},
	.enableContinuousSCK = false,
	.enableRxFifoOverWrite = false,
	.enableModifiedTimingFormat = false,
	.samplePoint = kDSPI_SckToSin0Clock,
};
}
//*/

static void
spi_proto_task(void *pvParameters)
{
	using namespace spi_proto;
	//setup SPI
	dspi_transfer_t slaveXfer;
	uint32_t i;
	callback_message_t cb_msg;

	// init p, set up buffers and len
	spi_proto_slave_initialize(&spi_proto::p);
	p.buflen = TRANSFER_SIZE;
	p.sendbuf = slaveSendBuffer;
	p.getbuf = slaveReceiveBuffer;
	spi_proto::ready = true;

	cb_msg.sem = xSemaphoreCreateBinary();
	dspi_sem = cb_msg.sem;
	if (cb_msg.sem == NULL) {
		PRINTF("DSPI slave: Error creating semaphore\r\n");
		vTaskSuspend(NULL);
	}
	/*Set up the transfer data*/
	for (i = 0; i < TRANSFER_SIZE; i++) {
		slaveSendBuffer[i] = 0xaa; /* this should be set by another process that needs to send something */
		slaveReceiveBuffer[i] = 0;
	}

	/*  Set dspi slave interrupt priority higher. */
	NVIC_SetPriority(EXAMPLE_SPI_SLAVE_IRQ, 5);

	DSPI_SlaveInit(EXAMPLE_DSPI_SLAVE_BASEADDR, &slaveConfig);

	/* Set up slave first */
	DSPI_SlaveTransferCreateHandle(EXAMPLE_DSPI_SLAVE_BASEADDR, &g_s_handle, DSPI_SlaveUserCallback, &cb_msg);

	//PRINTF("SPI set up\n");

	/* of course only the relative order matters, but conceptually it is better
	 * to think of checking for info to send, preparing the message, and then
	 * using the response
	 */
	for (;;) {
		slave_do_tick(p); // handles at least the functions below up to the semaphore

		/*Set slave transfer ready to receive/send data*/
		slaveXfer.txData = slaveSendBuffer;
		slaveXfer.rxData = slaveReceiveBuffer;
		slaveXfer.dataSize = 36;assert(TRANSFER_SIZE == 36);//TRANSFER_SIZE; TODO fix this hardwiring
		slaveXfer.configFlags = kDSPI_SlaveCtar0;

		DSPI_SlaveTransferNonBlocking(EXAMPLE_DSPI_SLAVE_BASEADDR, &g_s_handle, &slaveXfer);

		//PRINTF("SPI transfer started");
		xSemaphoreTake(cb_msg.sem, portMAX_DELAY);
		//TODO replace send buffer with an invalid message, so that an early transaction at least won't screw up the protocol
		//PRINTF("SPI recvd\n");
		spi_proto::spi_transactions++;

		//TODO handle the received message
		slave_spi_proto_rcv_msg(p, p.getbuf, p.buflen);
		//slave_get_message(p, slaveReceiveBuffer, TRANSFER_SIZE);
	}
	vTaskSuspend(NULL);
}


/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
static void hello_task(void *pvParameters) {
  for (;;) {
	/*PRINTF("Hello world.\r\n");*/
	/* Add your code here */
    //vTaskSuspend(NULL);
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

  motor_off();

  polling_init();
  BaseType_t ret;
  /* Create RTOS task */
  ret = xTaskCreate(hello_task, "Hello_task", configMINIMAL_STACK_SIZE, NULL, hello_task_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  ret = xTaskCreate(pin_hr_task, "pin heartrate task", configMINIMAL_STACK_SIZE+200, NULL, hello_task_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);

  ret = xTaskCreate(spi_proto_task, "spi proto task", configMINIMAL_STACK_SIZE+200, NULL, hello_task_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  ret = xTaskCreate(button_task, "button task", configMINIMAL_STACK_SIZE + 1000, NULL, hello_task_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  ret = xTaskCreate(solenoid_task, "solenoid task", configMINIMAL_STACK_SIZE+1000, NULL, hello_task_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  //ret = xTaskCreate(motor_task, "motor task", configMINIMAL_STACK_SIZE, NULL, hello_task_PRIORITY, NULL);
  //assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  ret = xTaskCreate(polling_task, "polling task", configMINIMAL_STACK_SIZE + 1000, NULL, hello_task_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  ret = xTaskCreate(flow_sensor_task, "flow sensor task", configMINIMAL_STACK_SIZE + 1000, NULL, hello_task_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  ret = xTaskCreate(lung_task, "lung task", configMINIMAL_STACK_SIZE, NULL, hello_task_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  ret = xTaskCreate(seattle_task, "seattle task", configMINIMAL_STACK_SIZE, NULL, hello_task_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);

  vTaskStartScheduler();

  for(;;) { /* Infinite loop to avoid leaving the main function */
    __asm("NOP"); /* something to use as a breakpoint stop while looping */
  }
}



