#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "fsl_adc16.h"
/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "fsl_debug_console.h"

/* entropic */
#include "spi_proto.h"
#include "spi_proto_slave.h"
#include "spi_edma_task.h"

/* included just for tasks */
#include "solenoid.h"
#include "ammdk-carrier/carrier_gpio.h"
#include "ammdk-carrier/solenoid.h"
#include "controllers/gpio.h"
#include "pressuresensor.h"
#include "flowsensor.h"

/* Task priorities. */
#define max_PRIORITY (configMAX_PRIORITIES - 1)

#define IVC_STATUS_WAITING  0
#define IVC_STATUS_START    1
#define IVC_STATUS_PAUSE    2
#define IVC_STATUS_STOP     3
#define IVC_STATUS_RESET    4

struct gpio_pin *rail_24v = &carrier_gpios[15];

unsigned int bpm_to_ms(unsigned int bpm_)
{
    float bpm = bpm_; // 1/m
    float bps = bpm/60; //1/m * m/s = 1/s
    float spb = 1/bps; // 1/1/s = s/1
    float mspb = 1000*spb;
    return mspb;
}

bool chest_rise_waiting = 1;
unsigned int breath_bpm = 12; // breaths per minute
unsigned int breath_dur = 5000; //ms
void chest_rise_task(void *pvParameters)
{
    struct solenoid::solenoid &left_intake = solenoids[7];
    struct solenoid::solenoid &right_intake = solenoids[6];
    struct solenoid::solenoid &left_exhaust = solenoids[5];
    struct solenoid::solenoid &right_exhaust = solenoids[4];
    //enable 24V rail
    GPIO_SetPinsOutput(GPIOA, 1U<<7U);
    //module logic
    // wait for start message
    // start an air intake by driving left_intake and right intake
    // monitor the left and right pressure
    // wait for breath_dur/5 seconds
    // turn off left and right intake
    // turn on left and right exhaust
    // wait for 2*breath_dur/5 seconds
    // turn off left and right exhaust
    // wait for 2*breath_dur/5 seconds to last breath_rate total

    //initialize solenoids to known state
    solenoid::off(left_exhaust);
    solenoid::off(right_exhaust);
    solenoid::off(left_intake);
    solenoid::off(right_intake);
    for (;;) {
        breath_dur = bpm_to_ms(breath_bpm);
        //wait for start message
        //TODO use a semaphore or task notification

        solenoid::on(left_intake);
        solenoid::on(right_intake);
        // wait 1.0 seconds
        vTaskDelay(pdMS_TO_TICKS (breath_dur /5));

        solenoid::off(left_intake);
        solenoid::off(right_intake);
        solenoid::on(left_exhaust);
        solenoid::on(right_exhaust);
        vTaskDelay(pdMS_TO_TICKS ((2 * breath_dur) /5));
        solenoid::off(left_exhaust);
        solenoid::off(right_exhaust);
        vTaskDelay(pdMS_TO_TICKS ((2 * breath_dur) /5));
        chest_rise_waiting = 1;
    }
}


void
bleeder_spi_cb(struct spi_packet *p)
{
    //TODO handle mule 1 stuff for IVC
    if (p->msg[0]) {
        switch (p->msg[1]) {
        case IVC_STATUS_START:
            //start task, should resume after a pause
            chest_rise_waiting = 0;
            break;
        case IVC_STATUS_RESET:
            //also reset flow
            total_pulses = 0;
        case IVC_STATUS_PAUSE:
        case IVC_STATUS_STOP:
        case IVC_STATUS_WAITING:
        default:
            //stop pressurizing but do not reset flow
            chest_rise_waiting = 1;
            break;
        }
        breath_bpm = p->msg[1];
    }
}

//fluid manager module code
int main(void) {
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    //enable 24V rail
    gpio_on(rail_24v);

    polling_init();
    BaseType_t ret;

    ret = xTaskCreate(spi_edma_task, "spi edma task", configMINIMAL_STACK_SIZE+200, (void*)bleeder_spi_cb, max_PRIORITY, NULL);
    assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);

    //ret = xTaskCreate(carrier_pressure_task, "carrier_pressure_task", configMINIMAL_STACK_SIZE + 100, NULL, max_PRIORITY-1, NULL);
    //assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);

    ret = xTaskCreate(chest_rise_task, "chest_rise_task", configMINIMAL_STACK_SIZE + 100, NULL, max_PRIORITY-1, NULL);
    assert(ret != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY);

    vTaskStartScheduler();

    for(;;) { /* Infinite loop to avoid leaving the main function */
        __asm("NOP"); /* something to use as a breakpoint stop while looping */
    }
}
