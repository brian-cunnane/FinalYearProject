/*
 * TemperatureSensor.h
 *
 *  Created on: 24 Apr 2016
 *      Author: brian
 */

#ifndef SOURCES_TEMPERATURESENSOR_H_
#define SOURCES_TEMPERATURESENSOR_H_

#define TEMPERATURECHANNEL 15

float readTemperature();

float readTemperature()
{

	ADC0_SC1A = 0x04Fu;
	ADC0_SC1A &= 0xFFFFFFE0;
	ADC0_SC1A |= TEMPERATURECHANNEL;
	int sample = 0;
	float vtemp = 0;
	float temp = 0;
	float averageTemp = 0;
	int i;
	for(i = 0; i < 256; i ++)
	{
		sample = read_adc0(TEMPERATURECHANNEL);
		vtemp = (float)sample/65536 * 3.3;
		temp = (vtemp) * 100;
		averageTemp += temp;
	}
	averageTemp = averageTemp/256;

	return averageTemp;
}

#endif /* SOURCES_TEMPERATURESENSOR_H_ */
