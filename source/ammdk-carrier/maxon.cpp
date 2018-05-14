#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_dac.h"


void
maxon_init(void)
{
	//B1 is motor enable (GPIO)
	//B4 is motor speed feedback (ADC)
	//B2 is motor status feedback (GPIO)
	//DAC0 is motor DAC speed control
	
    //leave 24V source off
    GPIO_ClearPinsOutput(GPIOA, 1U<<7U);
	
	//setup B1 as GPIO out
    port_pin_config_t motor_enable_settings = {0};
    motor_enable_settings.pullSelect = kPORT_PullDown;
    motor_enable_settings.mux = kPORT_MuxAsGpio;
    CLOCK_EnableClock(kCLOCK_PortB);
    PORT_SetPinConfig(PORTB, 1U, &motor_enable_settings);
    gpio_pin_config_t digital_out = {kGPIO_DigitalOutput, (0)};
    GPIO_PinInit(GPIOB, 1U, &digital_out);
	//setup B2 as GPIO in
    port_pin_config_t motor_status_settings = {0};
    motor_status_settings.pullSelect = kPORT_PullUp;
    motor_status_settings.mux = kPORT_MuxAsGpio;
    CLOCK_EnableClock(kCLOCK_PortB);
    PORT_SetPinConfig(PORTB, 2U, &motor_status_settings);
    gpio_pin_config_t digital_in = {kGPIO_DigitalInput, (0)};
    GPIO_PinInit(GPIOB, 2U, &digital_in);
	
	//set up DAC0 as demo dac
	dac_config_t dac0_config;
	DAC_GetDefaultConfig(&dac0_config);
	DAC_Init(DAC0, &dac0_config);
	DAC_SetBufferReadPointer(DAC0, 0U);
	/*
	//enable the adc on B4 (ADC1_SE10)
    adc16_config_t adc16ConfigStruct;
    ADC16_GetDefaultConfig(&adc16ConfigStruct);
    ADC16_Init(ADC1, &adc16ConfigStruct);
    ADC16_EnableHardwareTrigger(ADC1, false);
    ADC16_DoAutoCalibration(ADC1);

    adc16ChannelConfigStruct.channelNumber = 10U;
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
    adc16ChannelConfigStruct.enableDifferentialConversion = false;
	*/
}
