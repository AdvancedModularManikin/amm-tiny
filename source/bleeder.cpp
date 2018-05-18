#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "fsl_adc16.h"
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
#include "pressuresensor.h"
#include "flowsensor.h"

/* Task priorities. */
#define max_PRIORITY (configMAX_PRIORITIES - 1)

/* IVC module has two tasks:
 * maintain pressure in vein at ~0.1 psi -- careful not to pop it
 * monitor amount of flow and send it to biogears
 */
float bleed_pressure = 0.125;
float vein_psi;
volatile int pressurization_delay = 2;
//TODO keep track of bleed amount and warn when level is low
void
bleed_task(void *pvParameters)
{
  struct solenoid::solenoid &vein_sol = solenoids[7];
  
  //enable 24V rail
  GPIO_SetPinsOutput(GPIOA, 1U<<7U);
  
  for (;;) {
    uint32_t adcRead = carrier_sensors[0].raw_pressure;
    vein_psi = ((float)adcRead)*(3.0/10280.0*16.0) - 15.0/8.0;
    if (vein_psi < bleed_pressure) {
      solenoid::on(vein_sol);
      vTaskDelay(pressurization_delay);
      solenoid::off(vein_sol);
    }
    vTaskDelay(50);
  }
  vTaskSuspend(NULL);
}

void
bleeder_spi_cb(struct spi_packet *p)
{
  //TODO handle mule 1 stuff for IVC
  if (p->msg[0]) {
  }
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

  ret = xTaskCreate(bleed_task, "bleed task", configMINIMAL_STACK_SIZE+100, NULL, max_PRIORITY-1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  
  ret = xTaskCreate(flow_sensor_task, "flow task", configMINIMAL_STACK_SIZE+100, NULL, max_PRIORITY-1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  
  ret = xTaskCreate(spi_edma_task, "spi edma task", configMINIMAL_STACK_SIZE+200, (void*)bleeder_spi_cb, max_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  
  ret = xTaskCreate(carrier_pressure_task, "carrier_pressure_task", configMINIMAL_STACK_SIZE + 100, NULL, max_PRIORITY-1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);

  //TODO bleed control task
  
  vTaskStartScheduler();

  for(;;) { /* Infinite loop to avoid leaving the main function */
    __asm("NOP"); /* something to use as a breakpoint stop while looping */
  }
}
