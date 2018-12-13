//ammdk hat 1.0

//gpios. TODO separate gpio, dac, adc indices by type
int 
	//J3_2-3 are UART, reserved
	
	j4 = gpio_D4,
	j5 = gpio_D5,
	j6 = gpio_D6,
	j7 = gpio_D7,
	j8 = gpio_A10,
	j9 = gpio_A11,
	j10 = gpio_A12,
	j11 = gpio_A13,
	
	//J12 is sensirion flow sensor. Requires driver on k66, reserved here
	
	//J13_2 is flow counter. requires interrupt driver on k66, reserved here
	
	//J14_1-4 are SPI, reserved
	
	//J15_2-3 are I2C, reserved

	j19_1 = gpio_B18,
	j19_2 = gpio_B19,
	j19_3 = gpio_B20,
	j19_4 = gpio_B21,
	j21_1 = gpio_B22,
	j21_2 = gpio_B23,
	j21_3 = gpio_C0;

	J22_2 ADC1_SE15_RC
	J22_3 ADC1_SE16_RC

	U3	Pressure 1	ADC1_SE11_RC
	U4	Pressure 2	ADC1_SE12_RC
	U5	Pressure 3	ADC1_SE13_RC
	U6	Pressure 4	ADC1_SE14_RC

	U7	MOTOR_ENABLE = gpio_B1
	U7	MOTOR_PWM = gpio_B0
	U7	MOTOR_STATUS = gpio_B2
	U7	MOTOR_SPEED = gpio_B4
	U7	MOTOR_IO = gpio_B3
