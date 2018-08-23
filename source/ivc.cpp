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
#include <string.h>
#include "pressuresensor.h"
#include "flowsensor.h"

/* Task priorities. */
#define max_PRIORITY (configMAX_PRIORITIES - 1)
//TODO tear this out and delete this file, linux-side IVC should control everything remotely
#if 0
#define IVC_STATUS_WAITING  0
#define IVC_STATUS_START    1
#define IVC_STATUS_PAUSE    2
#define IVC_STATUS_STOP     3
#define IVC_STATUS_RESET    4

/*
 * the task has two modes: waiting and running.
 * START moves it to running from whereever.
 * PAUSE and STOP and WAITING move it to waiting
 * RESET moves it to waiting and resets the total_counts flow variable.
 */

/* IVC module has two tasks:
 * maintain pressure in vein at ~0.1 psi -- careful not to pop it
 * monitor amount of flow and send it to biogears
 */
float bleed_pressure = 0.125;
float vein_psi;
volatile int pressurization_quantum = 20;

bool ivc_waiting = 1;
void
bleed_task(void *pvParameters)
{
  struct solenoid::solenoid &vein_sol = solenoids[7];
  
  //enable 24V rail
  GPIO_SetPinsOutput(GPIOA, 1U<<7U);
  
  //module logic:
  //wait for start message
  //begin pressurizing
  //when pressurized, stop pressurizing and send the "I'm sealed" message to SoM code
  for (;;) {
    //wait for start message
    //TODO use a semaphore or task notification
    while (ivc_waiting)
      vTaskDelay(100);
    
    uint32_t adcRead = carrier_sensors[0].raw_pressure;
    vein_psi = ((float)adcRead)*(3.0/10280.0*16.0) - 15.0/8.0;
    if (vein_psi < bleed_pressure) {
      solenoid::on(vein_sol);
      do {
        if (ivc_waiting) {
          solenoid::off(vein_sol);
          while (ivc_waiting) vTaskDelay(50);
          solenoid::on(vein_sol);
        }
        vTaskDelay(pressurization_quantum);
        adcRead = carrier_sensors[0].raw_pressure;
        vein_psi = ((float)adcRead)*(3.0/10280.0*16.0) - 15.0/8.0;
      } while(vein_psi < bleed_pressure);
      solenoid::off(vein_sol);
    }
    
    char msg[32];
    msg[0] = 1;
    msg[1] = 1;
    slave_send_message(spi_proto::p, (unsigned char*) msg, 32);
    ivc_waiting = 1;
    vTaskDelay(50);
  }
  vTaskSuspend(NULL);
}
#endif
void
bleeder_spi_cb(struct spi_packet *p)
{
#if 0
  //TODO do this using generic protocol
  //TODO handle mule 1 stuff for IVC
  if (p->msg[0]) { // TODO proper spi_proto has "real?" tags so this should no longer be necessary, filler messages are no longer received
    switch (p->msg[1]) {
    case IVC_STATUS_START:
      //start task, should resume after a pause
      ivc_waiting = 0;
      break;
    case IVC_STATUS_RESET:
      //also reset flow
      total_pulses = 0;
    case IVC_STATUS_PAUSE:
    case IVC_STATUS_STOP:
    case IVC_STATUS_WAITING:
    default:
      //stop pressurizing but do not reset flow
      ivc_waiting = 1;
      break;
    }
  }
#endif
}
//#endif

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

  //ret = xTaskCreate(bleed_task, "bleed task", configMINIMAL_STACK_SIZE+100, NULL, max_PRIORITY-1, NULL);
  //assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  
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
