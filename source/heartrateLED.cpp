/* this file uses raw spi datagrams (not fully raw packets) */
#include "board.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"

/* freescale drivers */
#include "fsl_adc16.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* entropic */
#include "spi_edma_task.h"
#include "spi_proto.h"
#include "spi_proto_slave.h"

/* included just for tasks */
#include "ammdk-carrier/carrier_gpio.h"
#include "controllers/gpio.h"
#include "pressuresensor.h"

/* FreeRTOS Task priorities. */
#define max_PRIORITY (configMAX_PRIORITIES - 1)

// module variables
float heartrate = 600; /* ludicrous default to tell when update is received */
void heartratespicb(struct spi_packet *p)
{
  // handle heartrate stuff
  /* message structure:
   * spi_send[0] = heartrate;
   */
  heartrate = p->msg[0];
}

/* We receive the data in bpm, but we need ms wavelength to control the blink
 */
int ms_delay_from_cycle_per_minute(float rate)
{
  float bpm = rate;
  float bps = bpm / 60.0;
  float spb = 1 / bps;
  float mspb = 1000 * spb;
  return mspb;
}

/* We know it's 17 from looking at the chart in carrier_gpio.cpp
 */
#define GPIO_PIN_A24 carrier_gpios[17]

// A24, low is on
struct gpio_pin *heartrate_led_gpio = &GPIO_PIN_A24; // it's on the mainboard

void heartrate_task(void *ignored)
{
  // blink LED
  TickType_t xFrequency = pdMS_TO_TICKS(100);
  TickType_t xLastWakeTime = xTaskGetTickCount();

  for (;;) {
    // clamp this to 12bpm to prevent stuff like sleeping for a year
    unsigned int heart_delay_time = ms_delay_from_cycle_per_minute(heartrate);
    /* We need to avoid delay(a long time) and this is a reasonable choice
     */
    if (heart_delay_time > 5000)
      heart_delay_time = 5000;
    /* We need a FreeRTOS Tick, not milliseconds */
    xFrequency = pdMS_TO_TICKS(heart_delay_time);
    /* this is the FreeRTOS API for more acccurate periodic wakeups */
    vTaskDelayUntil(&xLastWakeTime, xFrequency);

    /* The above code means this code is called every xFrequency so just toggle
     */
    gpio_toggle(heartrate_led_gpio);
  }
}

/* This task returns information about the blood pressure */
void pressure_reporter_task(void *ignored)
{
  unsigned char msgbuf[2];

  for (;;) {
    uint16_t sensor = carrier_sensors[0].raw_pressure;
    msgbuf[0] = sensor >> 8;
    msgbuf[1] = sensor;
    slave_send_message(spi_proto::p, msgbuf, 2);

    /* We don't care as much about the precise frequency, so we don't use the
     * more accurate API.
     */
    vTaskDelay(pdMS_TO_TICKS(800));
  }
}

// heart rate manager module
int main(void)
{
  /* Init board hardware. */
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();

  /* Init polling pressure subsystem */
  polling_init();
  /* A variable to hold the return value of our task creations. */
  BaseType_t ret;
  
  /* Create RTOS tasks */
  ret = xTaskCreate
    ( spi_edma_task                  // The function that our task will execute
    , "spi edma task"                // A name for this task
    , configMINIMAL_STACK_SIZE + 200 // Stack size of the task, in words
    , (void *)heartratespicb         // An argument the task will be called with
    , max_PRIORITY                   // RTOS priority of the task
    , NULL                           // This argument can be used for returning 
                                     // a task handle. We don't need it now
    );
  /* Crash if the task wasn't created. This is deterministic. */
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);

  ret = xTaskCreate(heartrate_task, "heartrate_task",
    configMINIMAL_STACK_SIZE, NULL, max_PRIORITY - 1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);

  ret = xTaskCreate(pressure_reporter_task, "pressure_reporter_task",
    configMINIMAL_STACK_SIZE + 100, NULL, max_PRIORITY - 1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);

  ret = xTaskCreate(carrier_pressure_task, "carrier_pressure_task",
    configMINIMAL_STACK_SIZE + 100, NULL, max_PRIORITY - 1, NULL);
  assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);

  /* Start the FreeRTOS scheduler. */
  vTaskStartScheduler();

  for (;;) {      /* Infinite loop to avoid leaving the main function */
    __asm("NOP"); /* something to use as a breakpoint stop while looping */
  }
}
