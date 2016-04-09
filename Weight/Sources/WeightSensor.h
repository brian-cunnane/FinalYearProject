/*
 * WeightSensor.h
 *
 *  Created on: 9 Apr 2016
 *      Author: brian
 */

#ifndef SOURCES_WEIGHTSENSOR_H_
#define SOURCES_WEIGHTSENSOR_H_

unsigned long int calibrate();
unsigned long int readValue();
unsigned long int readAverageValue(unsigned long int av, unsigned long int zero_offset);

unsigned long int calibrate() {
	unsigned long int average = 0;
	int i;
	for (i = 0; i < 256; i++)
		average += readValue();
	average = average / 256;
	return average;
}

unsigned long int readValue() {
	int x;
	int i;
	unsigned long int sample = 0;
	char data[24] = { 0 };
	GPIOC_PCOR |= CLK_MASK; //PTC8 = 0
	while (DATA_read());
	for (x = 0; x < 24; x++) {
		GPIOC_PSOR |= CLK_MASK; //PTC8 = 1
		for (i = 0; i < 200; i++); //delay
		if (DATA_read()) {
			data[x] = 0x01u; //read data into char array to see binary value
			sample |= 0x01u; //read data into long int.
		}
		sample <<= 1;
		GPIOC_PCOR |= CLK_MASK;
		for (i = 0; i < 200; i++);
	}
	sample >>= 1;
	GPIOC_PSOR |= CLK_MASK;
	for (i = 0; i < 200; i++);
	GPIOC_PCOR |= CLK_MASK;
	sample ^= 0x800000;
	return sample;
}

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
