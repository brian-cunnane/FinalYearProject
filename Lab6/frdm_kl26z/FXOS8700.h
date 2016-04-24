/*
 * FXOS8700.h
 *
 *  Created on: 24 Nov 2015
 *      Author: nokeeffe
 *
 *	Header file for Freescale FXOS7800 IC
 */

#ifndef FRDM_KL26Z_FXOS8700_H_
#define FRDM_KL26Z_FXOS8700_H_

#include "i2c.h"

#define FXOS8700_ADDRESS 0x1D	//I2C slave address

//Register Addresses
#define CTRL_REG1_reg 0x2A
#define CTRL_REG4_reg 0x2D
#define WHO_AM_I_reg 0x0D
#define TEMPERATURE_reg 0x51
#define M_CTRL_REG1_reg 0x5B
#define M_CTRL_REG2_reg 0x5C
#define XYZ_DATA_CFG_reg 0x0E
#define OUT_X_MSB_reg 0x01
#define PL_STATUS_reg 0x10
#define PL_CFG_reg 0x11
#define PL_COUNT_reg 0x12

//function prototypes
void write_FXOS8700(unsigned char reg_address, unsigned char reg_data);
char read_FXOS8700(unsigned char reg_address);
void Pause(void);



#endif /* FRDM_KL26Z_FXOS8700_H_ */
