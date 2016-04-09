/*
 * WeightSensor.h
 *
 *  Created on: 9 Apr 2016
 *      Author: Brian Cunnane
 */

#ifndef SOURCES_WEIGHTSENSOR_H_
#define SOURCES_WEIGHTSENSOR_H_

unsigned long int calibrate();
unsigned long int readValue();
unsigned long int readAverageValue(unsigned long int av, unsigned long int zero_offset);

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


#endif /* SOURCES_WEIGHTSENSOR_H_ */
