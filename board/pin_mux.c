/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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

#include "fsl_device_registers.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "pin_mux.h"
#include "board.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Initialize all pins used in this example
 *
 * @param disablePortClockAfterInit disable port clock after pin
 * initialization or not.
 */
void BOARD_InitPins(void)
{
    /* Initialize UART0 pins below */
    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Affects PORTB_PCR16 register */
    PORT_SetPinMux(PORTB, 16u, kPORT_MuxAlt3);
    /* Affects PORTB_PCR17 register */
    PORT_SetPinMux(PORTB, 17u, kPORT_MuxAlt3);

    //green led
    port_pin_config_t led_green_settings = {0};
    led_green_settings.pullSelect = kPORT_PullUp;
    led_green_settings.mux = kPORT_MuxAsGpio;
    CLOCK_EnableClock(kCLOCK_PortE);
    PORT_SetPinConfig(PORTE, 6U, &led_green_settings);

	//TODO on application carrier board spi led is LED18 which is on some other pin. it's enabled here
	//SPI led is PTA24
	//GPIO_TogglePinsOutput(GPIOA, 1<<24);
    port_pin_config_t spi_led_settings = {0};
    spi_led_settings.pullSelect = kPORT_PullUp;
    spi_led_settings.mux = kPORT_MuxAsGpio;
    CLOCK_EnableClock(kCLOCK_PortA);
    PORT_SetPinConfig(PORTA, 24U, &spi_led_settings);
    GPIO_PinInit(GPIOA, 24U, &(gpio_pin_config_t){kGPIO_DigitalOutput, 0});

	CLOCK_EnableClock(kCLOCK_PortD);
    /* SPI on D0-D3 */

    PORT_SetPinMux(PORTD, 0U, kPORT_MuxAlt2);
    PORT_SetPinMux(PORTD, 1U, kPORT_MuxAlt2);
    PORT_SetPinMux(PORTD, 2U, kPORT_MuxAlt2);
    PORT_SetPinMux(PORTD, 3U, kPORT_MuxAlt2);

    //buttons
    port_pin_config_t button_settings = {0};
    button_settings.pullSelect = kPORT_PullUp;
    button_settings.mux = kPORT_MuxAsGpio;

    CLOCK_EnableClock(kCLOCK_PortA);
    PORT_SetPinConfig(BOARD_SW3_PORT, BOARD_SW3_GPIO_PIN, &button_settings);
    //strangely no SW2 defines in board.h
    PORT_SetPinConfig(PORTD, 11U, &button_settings);
	
	//I2C0 pins
	CLOCK_EnableClock(kCLOCK_PortC);
	port_pin_config_t i2c1_pin_config = {0};
	i2c1_pin_config.pullSelect = kPORT_PullUp;
	i2c1_pin_config.mux = kPORT_MuxAlt2;
	i2c1_pin_config.openDrainEnable = kPORT_OpenDrainEnable;
	PORT_SetPinConfig(PORTC, 10U, &i2c1_pin_config); // I2C0_SCL
	PORT_SetPinConfig(PORTC, 11U, &i2c1_pin_config); // I2C0_SDA
	
    port_pin_config_t source_24V_settings = {0};
    source_24V_settings.pullSelect = kPORT_PullDown;
    source_24V_settings.mux = kPORT_MuxAsGpio;
    CLOCK_EnableClock(kCLOCK_PortA);
    PORT_SetPinConfig(PORTA, 7U, &source_24V_settings);
	GPIO_PinInit(GPIOA, 7U, &(gpio_pin_config_t){kGPIO_DigitalOutput, 0});
}
