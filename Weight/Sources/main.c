/***********************************************************************************************
 *Author: Brian Cunnane
 *Date: 04/Apr/2016
 *Description: Weighing scales test code
 **********************************************************************************************/
#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "gpio.h"
#include "pit_kl26z.h"
#include "WeightSensor.h"

#define CONVERSIONFACTOR 20

/*****************************************************
 * Pseudocode description:
 * toggle clock pin 25 times
 * read data on datapin after each toggle
 * toggle clock once to reset
 * wait for data to go high before rereading
 */

//unsigned long int calibrate();
//unsigned long int readValue();
//unsigned long int readAverageValue(unsigned long int av);

unsigned long int zero_offset = 0;

int main(void) {
	int i;
	int value;
	int grams = 0;
	unsigned long int averageValue = 0;

	hardware_init();
	FRDM_KL26Z_CLK_Configure(); //configure PTC8 as output for clocking hx711
	FRDM_KL26Z_DATA_Configure(0, 0); //configure PTC9 as input for HX711

	PRINTF("\r\nWeighing scales test code\r\n");
	for (i = 0; i < 256; i++)
				averageValue += readValue();
	averageValue = averageValue / 256;
	PRINTF("Initial value %ld\r\n",averageValue);
	averageValue = 0;
	PRINTF("Calibrating\r\n");
	zero_offset = calibrate(zero_offset);
	PRINTF("\n\n\nZero offset from calibration: %ld\r\n\n\n", zero_offset);

	while (1) {
		averageValue = readAverageValue(averageValue, zero_offset);
		grams = averageValue/CONVERSIONFACTOR;
		PRINTF("\r\nSample is %ldg. \r\n", grams);
		averageValue = 0;
	}
	return 0;
}
/*
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

unsigned long int readAverageValue(unsigned long int av){
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
*/
