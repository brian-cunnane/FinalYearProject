/*
 * i2c.c
 *
 *  Created on: 24 Nov 2015
 *      Author: nokeeffe
 *
 * 	Low-level functions for KL26Z I2C0 module
 */

#include "i2c.h"

/********************************************************************
 *	Function to configure and enable the I2C0 module
 *	I2C clock frequency is 600KHz
 *********************************************************************/
void i2c0_configure()
{
	SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;	//enable module clock

	//configure port pins for I2C operation
	PORTE_PCR24 |= (0x5<<8) | 0x3;	//Alt5 + pullup enabled
	PORTE_PCR25 |= (0x5<<8) | 0x3;	//Alt5 + pullup enabled

	//I2C baud rate = peripheral bus clock / (mul * SCL divider)
	//Bus clock = 48MHz
	I2C0_F = 0x14;	//mul=1, SCL = 80

	I2C0_C1 = 0x80;	//enable I2C module - C1[IICEN]] = 1

}

/*******************************************************************
 * Function to generate an I2C start condition
 * C1[TX] = 1
 * C1[MST] = 1
 *******************************************************************/
void i2c_start()
{
	I2C0_C1 |= 0x10;	//Transmit mode select
	I2C0_C1 |= 0x20;	//Generate start bit
}

/*******************************************************************
 * Function to generate an I2C stop condition
 * C1[MST] = 0
 * C1[TX] = 0
 *******************************************************************/
void i2c_stop()
{
	I2C0_C1 &= 0xDF;	//Generate stop bit
	I2C0_C1  &= 0xEF;	//Receive mode select
}

/*******************************************************************
 * Function to generate an I2C repeat start condition
 * This allows the master to retain the I2C bus
 *******************************************************************/
void i2c_repeat_start()
{
	I2C0_C1 |= 0x04;	//set C1[RSTA] bit
}

/*******************************************************************
 * Function to wait for an acknowledge bit after data transmission/reception
 *******************************************************************/
void i2c_wait_for_ack()
{
	while((I2C0_S & 0x02)==0)	//wait for interrupt flag S1[IICIF]
	{}
	I2C0_S |= 0x20;	//clear interrupt flag

}



