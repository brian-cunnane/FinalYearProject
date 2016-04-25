/*
 * FXOS8700.c
 *
 *  Created on: 24 Nov 2015
 *      Author: nokeeffe
 *
 * High-level functions to read from and write to the Freescale FXOS7800 Accelerometer and Magnetometer IC
 */

#include "FXOS8700.h"

/*************************************************************************
 * Function to write data to a FXOS8700 register
 *
 * Passed Parameters: -
 * 1. Register address
 * 2. Data to be written to register
 *************************************************************************/
void write_FXOS8700(unsigned char reg_address, unsigned char reg_data)
{
	unsigned char address;
	i2c_start();
	address = (unsigned char)FXOS8700_ADDRESS << 1;
	I2C0_D = address & 0xFE;	//RW bit = 0
	i2c_wait_for_ack();

	//send register address and wait for acknowledgement
	I2C0_D = reg_address;
	i2c_wait_for_ack();

	//send data to device
	I2C0_D = reg_data;
	i2c_wait_for_ack();

	i2c_stop();
	Pause();
}
/*************************************************************************
 * Function to read data from a FXOS8700 register
 *
 * Passed Parameters: -
 * 1. Register address
 *
 * Returned Parameters
 * 1. Register Data
 *************************************************************************/
char read_FXOS8700(unsigned char reg_address)
{
	unsigned char rx_data, address;
	int i;
	i2c_start();
	address = (unsigned char)FXOS8700_ADDRESS << 1;
	I2C0_D = address & 0xFE;	//RW bit = 0
	I2C0_D = (unsigned char)FXOS8700_ADDRESS << 1;	//RW bit = 0
	i2c_wait_for_ack();

	//send register address and wait for acknowledgement
	I2C0_D = reg_address;
	i2c_wait_for_ack();

	//repeat start
	i2c_repeat_start();

	I2C0_D = (FXOS8700_ADDRESS << 1) | 0x01;	//RW bit = 1
	i2c_wait_for_ack();

	//change to receive mode and turn off acknowledge
	I2C0_C1 &= 0xEF;	//clear TX flag to enter receive mode
	I2C0_C1 |= 0x08;	//Turn off acknowledge
	//I2C0_C1 &= 0xF7;	//clear TXAK flag

	rx_data = I2C0_D;	//dummy read to initiate reception
	for(i=0;i<5000;i++);	//delay to allow slave to respond
	i2c_wait_for_ack();
	i2c_stop();

	rx_data = I2C0_D;	//read data
	Pause();
	return rx_data;

}

/*******************************************************************/
/*!
 * Pause Routine
 */
void Pause(void){
    int n;
    for(n=1;n<500;n++) {
      //asm("nop");
    }
}
