/*
 * i2c.h
 *
 *  Created on: 24 Nov 2015
 *      Author: nokeeffe
 *
 *	Header file for KL26Z I2C0
 */

#ifndef FRDM_KL26Z_I2C_H_
#define FRDM_KL26Z_I2C_H_

#include "fsl_device_registers.h"

//Function Prototypes
void i2c0_configure();
void i2c_start();
void i2c_stop();
void i2c_repeat_start();
void i2c_wait_for_ack();




#endif /* FRDM_KL26Z_I2C_H_ */
