#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* entropic */
#include "spi_proto.h"
#include "spi_proto_slave.h"
#include "spi_edma_task.h"

/* included just for tasks */
#include "solenoid.h"
#include "ammdk-carrier/solenoid.h"
#include <string.h>

/* Task priorities. */
#define max_PRIORITY (configMAX_PRIORITIES - 1)

int arterial_bleeding = 0;
uint16_t arterial_bleed_period = 1000;
//TODO keep track of bleed amount and warn when level is low
void
bleed_task(void *pvParameters)
{
  for (;;) {
    if (arterial_bleed_period < 100) {
      arterial_bleed_period = 1000;
      arterial_bleeding = 0;
    }
    if (arterial_bleeding) {
      solenoid::toggle(solenoids[7]);
      //TODO if somehow this delays for a long time it will lock up. perhaps delay in shorter segments.
      vTaskDelay(arterial_bleed_period);
    } else {
      solenoid::off(solenoids[7]);
      vTaskDelay(50);
    }
  }
  vTaskSuspend(NULL);
}

void
bleeder_spi_cb(struct spi_packet *p)
{
  //TODO handle mule 1 stuff for IVC
  if (p->msg[0]) {
    arterial_bleeding = p->msg[1];
    memcpy(&arterial_bleed_period, &p->msg[2], 2);
  }
}

//fluid manager module code
int main(void) {
  /* Init board hardware. */
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();

  BaseType_t ret;
  /* Create RTOS task */

  ret = xTaskCreate(solenoid_gdb_mirror_task, "solenoid_gdb_mirror_task", configMINIMAL_STACK_SIZE+100, NULL, max_PRIORITY-1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);

  ret = xTaskCreate(bleed_task, "bleed task", configMINIMAL_STACK_SIZE+100, NULL, max_PRIORITY-1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  
  ret = xTaskCreate(flow_sensor_task, "flow task", configMINIMAL_STACK_SIZE+100, NULL, max_PRIORITY-1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  
  ret = xTaskCreate(spi_edma_task, "spi edma task", configMINIMAL_STACK_SIZE+200, (void*)bleeder_spi_cb, max_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);

  //TODO bleed control task
  
  vTaskStartScheduler();

  for(;;) { /* Infinite loop to avoid leaving the main function */
    __asm("NOP"); /* something to use as a breakpoint stop while looping */
  }
}
