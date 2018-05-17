#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

/* freescale drivers */
#include "fsl_dspi.h"
#include "fsl_dspi_freertos.h"
#include "fsl_adc16.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* entropic */
#include "spi_proto.h"
#include "spi_proto_slave.h"
#include "spi_edma_task.h"

/* included just for tasks */
#include "sensirion.h"
#include "maxon.h"
#include <stdio.h>
#include "mule-1/air_tank.h"
#include "solenoid.h"
#include "pressuresensor.h"

/* Task priorities. */
#define max_PRIORITY (configMAX_PRIORITIES - 1)

void
fluidmanagerspicb(struct psi_packet *p)
{
  //TODO
}

//fluid manager module code
int main(void) {
  /* Init board hardware. */
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();

  polling_init();
  BaseType_t ret;
  /* Create RTOS task */

  ret = xTaskCreate(solenoid_gdb_mirror_task, "solenoid_gdb_mirror_task", configMINIMAL_STACK_SIZE+100, NULL, max_PRIORITY-1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  
  ret = xTaskCreate(spi_edma_task, "spi edma task", configMINIMAL_STACK_SIZE+200, (void*)fluidmanagerspicb, max_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);

  ret = xTaskCreate(carrier_pressure_task, "carrier_pressure_task", configMINIMAL_STACK_SIZE + 100, NULL, max_PRIORITY-1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  
  ret = xTaskCreate(air_reservoir_control_task, "airtank control", configMINIMAL_STACK_SIZE + 100, NULL, max_PRIORITY-1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);

  ret = xTaskCreate(maxon_task, "Maxon task", configMINIMAL_STACK_SIZE + 100, NULL, max_PRIORITY-1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  
  vTaskStartScheduler();

  for(;;) { /* Infinite loop to avoid leaving the main function */
    __asm("NOP"); /* something to use as a breakpoint stop while looping */
  }
}
