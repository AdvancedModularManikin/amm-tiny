J16 Pin Number	Pin Name	Generic Application Board Pin Function	Usage
1	PTD6_RC	Soleonid 3 Control	Set high to activate solenoid
2	GND	Common Ground	
3	PTD4_RC	Solenoid 1 Control	Set high to activate solenoid
4	PTD5_RC	Solenoid 2 Control	Set high to activate solenoid
5	PTC6_RC	Solenoid 12 Control	Set high to activate solenoid
6	PTC9_RC	High volume flow sensor	Count pulses to determine flow rate
7	PTA4_RC	Solenoid 13 Control	Set high to activate solenoid
8	PTC0_RC	GPIO 7	3.3 V max
9	PTB1_RC	Motor Enable	Set high to activate motor
10	PTC1_RC	Solenoid 10 Control	Set high to activate solenoid
11	GND	Common Ground	
12	PTA8_RC	Solenoid 11 Control	Set high to activate solenoid
13	PTB4_RC	Motor Speed Feedback (Analogue)	Read voltage level to determine motor speed
14	PTB3_RC	Motor IO	General purpose motor IO
15	ADC1_SE15_RC	Analogue Input 1	Read voltage level (3.3 V max)
16	ADC1_SE14_RC	Pressure Sensor 4 (Analogue)	Read voltage level to determine pressure
17	PTB6_RC	Pressure Sensor 2 (Analogue)	Read voltage level to determine pressure
18	PTB7_RC	Pressure Sensor 3 (Analogue)	Read voltage level to determine pressure
19	PTB2_RC	Motor Status Feedback	
20	PTB5_RC	Pressure Sensor 1 (Analogue)	Read voltage level to determine pressure
21	PTB0_RC	Motor PWM Speed Control	Modulate pulse width to set motor speed
22	PTA11_RC	Solenoid 6 Control	Set high to activate solenoid (PWM capable)
23	PTA13_RC	J15 I2C Header SDA	I2C2, 3.3 V max
24	PTA14_RC	J3 UART TX	UART transmit (3.3 V)
25	PTA17_RC	Solenoid 14 Control	Set high to activate solenoid
26	PTA15_RC	J3 UART RX	UART receive (3.3 V max)
27	PTA10_RC	Solenoid 5 Control	Set high to activate solenoid (PWM capable)
28	GND	Common Ground	
29	ADC0_SE18_RC	Pressure Sensor 5 (Analogue)	Read voltage level to determine pressure
30	PTA12_RC	J15 I2C Header SCL	I2C2, 3.3 V max
31	ADC1_SE18_RC	Pressure Sensor 6 (Analogue)	Read voltage level to determine pressure
32	PTA7_RC	Solenoid 9 Control	Set high to activate solenoid
33	ADC1_SE16_RC	Analogue Input 2	Read voltage level (3.3 V max)
34	ADC0_SE17_RC	Solenoid 7 Control	Set high to activate solenoid
35	ADC1_DP0_RC	Pressure Sensor 7 (Analogue)	Read voltage level to determine pressure
36	ADC0_SE16_RC	N/A	
37	ADC0_DM0_RC	Pressure Sensor 8 (Analogue)	Read voltage level to determine pressure
38	ADC1_DM0_RC	N/A	
39	GND	Common Ground	
40	RST_TGTMCU_B	N/A	


J17 Pin Number	Pin Name	Generic Application Board Pin Function	Usage
1	PTD11	Solenoid 15 Control	Set high to activate solenoid
2	GND	Common Ground	
3	PTD12	Solenoid 16 Control	Set high to activate solenoid
4	PTC14	N/A	
5	I2C0_SDA	N/A	
6	FTM0_CH3	N/A	
7	I2C0_SCL	N/A	
8	I2C1_SDA	Low Flow Sensor I2C SDA	I2C1, 3.3 V max
9	PTD7	Solenoid 4 Control	Set high to activate solenoid
10	I2C1_SCL	Low Flow Sensor I2C SCL	I2C1, 3.3 V max
11	PTC15	N/A	
12	FTM0_CH2	N/A	
13	GND	Common Ground	
14	FTM0_CH1	N/A	
15	PTB21	GPIO 4	3.3 V max
16	PTB23	GPIO 6	3.3 V max
17	PTB20	GPIO 3	3.3 V max
18	PTB22	GPIO 5	3.3 V max
19	PTB18	GPIO 1	3.3 V max
20	PTB19	GPIO 2	3.3 V max
21	SDHC0_DCLK	SPI SCLK	SPI1, 3.3 V max
22	SD_CARD_DETECT	N/A	
23	SDHC0_D1	N/A	
24	SDHC0_D0	SPI MOSI	SPI1, 3.3 V max
25	SDHC0_D3	SPI SS	SPI1, 3.3 V max
26	SDHC0_CMD	SPI MISO	SPI1, 3.3 V max
27	PTE6	N/A	
28	SDHC0_D2	N/A	
29	PTE8	N/A	
30	PTE7	N/A	
31	PTE11	N/A	
32	PTE12	Solenoid 8 Control	Set high to activate solenoid
33	CAN0_H	N/A	
34	CAN0_L	N/A	
35	DAC0_OUT_RC	N/A	
36	DAC1_OUT_RC	Motor Speed Input (Analogue)	Set an analogue voltage to control motor speed
37	PTE26	N/A	
38	PTE28	N/A	
39	GND	Common Ground	
40	RESERVED FOR INTERNAL USE	N/A	



SOLENOID CONTROL
Descriptor	Pin 	Function	Motherboard MCU Pin Name	Notes
J4	8	Solenoid 1 GND (N-FET Controlled)	PTD4_RC	500 mA max
J4	7	Solenoid 2 GND (N-FET Controlled)	PTD5_RC	500 mA max
J4	6	Solenoid 3 GND (N-FET Controlled)	PTD6_RC	500 mA max
J4	5	Solenoid 4 GND (N-FET Controlled)	PTD7	500 mA max
J4	4	Solenoid 5 GND (N-FET Controlled)	PTA10_RC	500 mA max, PWM Capable
J4	3	Solenoid 6 GND (N-FET Controlled)	PTA11_RC	500 mA max, PWM Capable
J4	2	Solenoid 7 GND (N-FET Controlled)	ADC0_SE17_RC	500 mA max
J4	1	Solenoid 8 GND (N-FET Controlled)	PTE12	500 mA max
/* J5 has no runtime controls */
J6	8	Solenoid 9 GND (N-FET Controlled)	PTA7_RC	500 mA max
J6	7	Solenoid 10 GND (N-FET Controlled)	PTC1_RC	500 mA max
J6	6	Solenoid 11 GND (N-FET Controlled)	PTA8_RC	500 mA max
J6	5	Solenoid 12 GND (N-FET Controlled)	PTC8_RC	500 mA max
J6	4	Solenoid 13 GND (N-FET Controlled)	PTA4_RC	500 mA max
J6	3	Solenoid 14 GND (N-FET Controlled)	PTA17_RC	500 mA max
J6	2	Solenoid 15 GND (N-FET Controlled)	PTD11	500 mA max
J6	1	Solenoid 16 GND (N-FET Controlled)	PTD12	500 mA max
/* J7 has no runtime controls */



PRESSURE SENSOR
"To calculate pressure, read the 12-bit ADC value, and use this equation:
Pressure (PSI) = VALUE * (3/10280*16) - (15/8)"	
Descriptor	Pressure Sensor Name	Motherboard MCU Pin Name	Notes		
U3	Pressure 1	PTB5_RC		
U4	Pressure 2	PTB6_RC			
U5	Pressure 3	PTB7_RC			
U6	Pressure 4	ADC1_SE14_RC			
U8	Pressure 5	ADC0_SE18_RC			
U9	Pressure 6	ADC1_SE18_RC			
U10	Pressure 7	ADC1_DP0_RC			
U11	Pressure 8	ADC0_DM0_RC			


//GPIO
J9_1 = gpio_B18 //PWM Capable
J9_2 = gpio_B19 //PWM Capable
J9_3 = gpio_B20
J9_4 = gpio_B21
J9_5 = gpio_B22
J9_6 = gpio_B23
J9_7 = gpio_C0 //Analogue Input Capable


MOTOR
Descriptor	Pin	Function Description	Interface	Motherboard MCU PIn
U7	MOTOR_ENABLE	Motor	gpio_B1
U7	MOTOR_PWM		Motor	gpio_B0
U7	MOTOR_STATUS	Motor	gpio_B2
U7	MOTOR_IO		Motor	gpio_B3
U7	MOTOR_SPEED_IN	Motor	DAC1_OUT_RC
U7	MOTOR_SPEED_OUT	Motor	gpio_B4

