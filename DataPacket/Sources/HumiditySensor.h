/*
 * HumiditySensor.h
 *
 *  Created on: 14 Apr 2016
 *      Author: brian
 */

#ifndef SOURCES_HUMIDITYSENSOR_H_
#define SOURCES_HUMIDITYSENSOR_H_

#define HUMIDITYCHANNEL 14

char readHumidity(float temp);

char readHumidity(float temp)
{
	ADC0_SC1A = 0x04Fu;
	ADC0_SC1A &= 0xFFFFFFE0;
	ADC0_SC1A |= HUMIDITYCHANNEL;
	float average = 0;
	for(int i = 0; i < 256; i ++){
		unsigned int sample = read_adc0(HUMIDITYCHANNEL);
		float relativeHumidity = (float) sample - 15887.27273;//32981.97818;
		relativeHumidity = relativeHumidity * 0.00162;
		average += relativeHumidity;
	}

	average = average/256;

	float realHumidity = average/(1.0546 - (0.00216 * temp));
	return (char)realHumidity;
}


#endif /* SOURCES_HUMIDITYSENSOR_H_ */
