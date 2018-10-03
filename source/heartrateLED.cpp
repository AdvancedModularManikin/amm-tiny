/* this file uses raw spi datagrams (not fully raw packets) */
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

/* freescale drivers */
#include "fsl_adc16.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* entropic */
#include "spi_proto.h"
#include "spi_proto_slave.h"
#include "spi_edma_task.h"

/* included just for tasks */
#include "controllers/gpio.h"

/* Task priorities. */
#define max_PRIORITY (configMAX_PRIORITIES - 1)

//module variables
float heartrate = 600; /*ludicrous default to tell when update is received */
void
heartratespicb(struct spi_packet *p)
{
  // handle heartrate stuff
  /* message structure:
    spi_send[0] = heartrate;
  */
  
  heartrate = p->msg[0];
}

int
ms_delay_from_cycle_per_minute(float rate)
{
  return 1.0/(rate * (1/60) * 0.001);
}

struct gpio_pin *heartrate_led_gpio; // TODO select one of the LEDs on the main board

void
heartrate_task(void *ignored)
{
  //blink LED
  TickType_t xFrequency = pdMS_TO_TICKS(100);
  TickType_t xLastWakeTime = xTaskGetTickCount();

  for( ;; ) {
    //TODO clamp this to prevent stuff like sleeping for a year
    int heart_delay_time = pdMS_TO_TICKS(ms_delay_from_cycle_per_minute(heartrate));
    xFrequency = pdMS_TO_TICKS(heart_delay_time);
    vTaskDelayUntil( &xLastWakeTime, xFrequency );

    gpio_toggle(heartrate_led_gpio);
  }
}

//heart rate manager module 
int main(void) {
  /* Init board hardware. */
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();

  //polling_init();
  BaseType_t ret;
  /* Create RTOS task */

  /*
  ret = xTaskCreate(solenoid_gdb_mirror_task, "solenoid_gdb_mirror_task", configMINIMAL_STACK_SIZE+100, NULL, max_PRIORITY-1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  */
  
  ret = xTaskCreate(spi_edma_task, "spi edma task", configMINIMAL_STACK_SIZE+200, (void*)heartratespicb, max_PRIORITY, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);

  ret = xTaskCreate(heartrate_task, "heartrate_task", configMINIMAL_STACK_SIZE, NULL, max_PRIORITY-1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  
  /*
  ret = xTaskCreate(carrier_pressure_task, "carrier_pressure_task", configMINIMAL_STACK_SIZE + 100, NULL, max_PRIORITY-1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  
  ret = xTaskCreate(air_reservoir_control_task, "airtank control", configMINIMAL_STACK_SIZE + 100, NULL, max_PRIORITY-1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);

  ret = xTaskCreate(maxon_task, "Maxon task", configMINIMAL_STACK_SIZE + 100, NULL, max_PRIORITY-1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);
  */
  
  vTaskStartScheduler();

  for(;;) { /* Infinite loop to avoid leaving the main function */
    __asm("NOP"); /* something to use as a breakpoint stop while looping */
  }
}
