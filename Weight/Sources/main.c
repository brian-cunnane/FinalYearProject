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

int main(void) {
	int i;
	int value;
	int grams = 0;
	unsigned long int zero_offset = 0;
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
