/*
 * Accelerometer.h
 *
 *  Created on: 24 Apr 2016
 *      Author: brian
 */

#ifndef SOURCES_ACCELEROMETER_H_
#define SOURCES_ACCELEROMETER_H_

#include "FXOS8700.h"
#include "i2c.h"
void configureAccelerometer();
char readAccelerometer();

void configureAccelerometer()
{
	i2c0_configure();
	write_FXOS8700(CTRL_REG1_reg, 0x00);//put accelerometer into standby mode
	write_FXOS8700(XYZ_DATA_CFG_reg, 0x01);//set tolerance to +- 4g
	write_FXOS8700(CTRL_REG1_reg, 0x0D);//put accelerometer into active mode
}
char readAccelerometer()
{
	char data[6];
	int x = 0;
	int y = 0;
	int z = 0;
	int val = 0;
	for(int i = 0; i < 6; i ++)
			data[i] = read_FXOS8700(OUT_X_MSB_reg + i); //read all axes 8 bit so no need for lsb reg
		x = (data[0]<<6)+(data[1]>>2);//<<6 for msb
		y = (data[2]<<6)+(data[3]>>2);
		z = (data[4]<<6)+(data[5]>>2);
		val= (2048 - (abs(z)));// 2048 = LSB/g
	return (char)val;
}


#endif /* SOURCES_ACCELEROMETER_H_ */
