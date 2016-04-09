/*
 * WeightSensor.h
 *
 *  Created on: 9 Apr 2016
 *      Author: Brian Cunnane
 */

#ifndef SOURCES_WEIGHTSENSOR_H_
#define SOURCES_WEIGHTSENSOR_H_

#define CLK_MASK 0x100				//ptc8
#define DATA_MASK 0x200				//ptc9

unsigned long int calibrate();
unsigned long int readValue();
unsigned long int readAverageValue(unsigned long int av, unsigned long int zero_offset);
char DATA_read();

/**************************************************************************************
 *
 * Sample and sum data 256 times.
 * Divide by number of samples.
 * Return average value which corresponds to the weight of the hardware i.e. the zero offset
 *
 **************************************************************************************/
unsigned long int calibrate() {
	unsigned long int average = 0;
	int i;
	for (i = 0; i < 256; i++)
		average += readValue();
	average = average / 256;
	return average;
}


/*************************************************************************************
 *
 * Set clock low.
 * Wait for data line to go low.
 * Set clock high.
 * Delay to allow settling
 * If data line is high, or 1 bit into sample value
 * Shift sample 1 bit
 * Set clock low
 * Delay
 * Shift sample one bit right to remove extra 0
 * Toggle clock to reset conversion
 * XOR MSB of sample with 1 to remove sign.
 *
 *************************************************************************************/
unsigned long int readValue() {
	int x;
	int i;
	unsigned long int sample = 0;
	GPIOC_PCOR |= CLK_MASK; //PTC8 = 0
	while (DATA_read());
	for (x = 0; x < 24; x++) {
		GPIOC_PSOR |= CLK_MASK; //PTC8 = 1
		for (i = 0; i < 200; i++); //delay
		if (DATA_read())
			sample |= 0x01u; //read data into long int.
		sample <<= 1;
		GPIOC_PCOR |= CLK_MASK;
		for (i = 0; i < 200; i++);
	}
	sample >>= 1;
	GPIOC_PSOR |= CLK_MASK;
	for (i = 0; i < 200; i++);
	GPIOC_PCOR |= CLK_MASK;
	sample ^= 0x800000;//data is in two's compliment so remove the sign.
	return sample;
}

/***************************************************************************************
 *
 * Sample and sum the data 256 times.
 * Minus the zero offset calculated during calibration.
 * Devide sum by 256 and return value
 *
 ***************************************************************************************/
unsigned long int readAverageValue(unsigned long int av, unsigned long int zero_offset){
	int i;
	unsigned long int value;
	for (i = 0; i < 256; i++){
				value = readValue();
				av = av + value - zero_offset;
				PRINTF("\r\n\t\t%ld",value);
			}
			av = av / 256;
			return av;
}

void FRDM_KL26Z_CLK_Configure()
{
	//enable port clock
	SIM_SCGC5 |= PORTC_CLK_ENABLE_MASK;
	PORTC_PCR8 |= GPIO_MUX_MASK;	//configure as GPIO pin
	GPIOC_PDDR |= CLK_MASK; //PTC8 output direction
	GPIOC_PSOR |= CLK_MASK; // PTC8 = 1;
}

void FRDM_KL26Z_DATA_Configure(int pull_resistor, int interrupt_option)
{
	//enable port clock
	SIM_SCGC5 |= PORTC_CLK_ENABLE_MASK;
	PORTC_PCR9 |= GPIO_MUX_MASK;	//configure as GPIO pin

	//configure pullup/pulldown resistor
	if(pull_resistor == PULLUP)
		PORTC_PCR9 |= PULLUP_ENABLED_MASK;	//PE=1, PS=1
	else if(pull_resistor == PULLDOWN)
		PORTC_PCR9 |= PULLDOWN_ENABLED_MASK;	//PE=1,PS=0

	//configure interrupt
	switch(interrupt_option)
	{
	case LOW_LEVEL: PORTC_PCR9 |= LOW_LEVEL_INTERRUPT_MASK;
	break;
	case RISING_EDGE: PORTC_PCR9 |= RISING_EDGE_INTERRUPT_MASK;
	break;
	case FALLING_EDGE: PORTC_PCR9 |= FALLNG_EDGE_INTERRUPT_MASK;
	break;
	case EITHER_EDGE: PORTC_PCR9 |= EITHER_EDGE_INTERRUPT_MASK;
	break;
	case HIGH_LEVEL: PORTC_PCR9 |= HIGH_LEVEL_INTERRUPT_MASK;
	break;
	default:
	break;
	}
}
char DATA_read()
{
	if (GPIOC_PDIR & DATA_MASK)
		return 1;
	return 0;
}


#endif /* SOURCES_WEIGHTSENSOR_H_ */
