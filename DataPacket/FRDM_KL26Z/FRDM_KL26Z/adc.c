/*
 * adc.c
 *
 *  Created on: 14 Oct 2015
 *      Author: nokeeffe
 */

#include "fsl_device_registers.h"
#include "adc.h"

/*************************************************************************
 * Configures ADC0 in 16-bit single-ended mode using software triggering
 *  *
 * Passed Parameters:
 * trigger -	ADC trigger mode
 *  			Options: SW_TRIGGER, HW_TRIGGER
 *
 * mode - 		number of bits in conversion sample
 *  			Options : BIT8, BIT10, BIT12, BIT16
 *
 * interrupt_config -	Enable/Disable ADC interrupt
 *  			Options: ADC_INTERRUPT_DISABLED, ADC_INTERRUPT_ENABLED
 *
 * The ADC is configured to use the bus clock.
 * ADC is configured in single conversion mode
 */
void adc0_config(char trigger, char mode, char interrupt_config)
{
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;	//enable ADC peripheral clock
	//Configure ADC registers
	ADC0_CFG2 = 0;		//Leave at default for low speed conversions
	ADC0_SC3 = 0;		//no calibration required
	ADC0_SC1A = 0;

	//interrupt configuration
	if(interrupt_config == ADC_INTERRUPT_ENABLED)
	{
		//Enable ADC interrupt in NVIC
		NVIC_ClearPendingIRQ(15);
		NVIC_EnableIRQ(15);
		ADC0_SC1A |= 0x40;	//set AIEN bit
	}

	//Mode configuration
	//clock div = 8, Clock set to bus clock
	//mode parameter selects 8,10,12,16 bit conversion
	ADC0_CFG1 = 0x70u | (mode << 2);	//clock div = 8,

	//configure ADC trigger mode
	//if using hardware trigger, the trigger source must be configured in the SIM_OPT7 register
	if (trigger == SW_TRIGGER)
		ADC0_SC2 &= SW_TRIGGER_MASK; 	//clear bit 6
	else if (trigger == HW_TRIGGER)
		ADC0_SC2 |= HW_TRIGGER_MASK;	//set bit 6

}

/*
 * Function to perform an ADC conversion when configured in Software Trigger mode
 *
 * Passed Parameters:
 * 	channel_no	ADC input channel to convert
 * 				Options on board are LIGHT_SENSOR_CHANNEL, POT_CHANNEL, TEMPERATURE_CHANNEL
 *
 * Function returns the sample value
 */
unsigned int read_adc0(char channel_no)
{
	//Configure channel and start conversion
	ADC0_SC1A &= 0xFFFFFFE0;	//clear channel to 0
	ADC0_SC1A |= channel_no;	//start conversion on channel

	//wait for end of conversion
	while((ADC0_SC1A & 0x80) == 0)
	{}

	return(ADC0_RA);
}

/*
 * Function to read from temperature sensor on the ADC module
 * Temperature sensor is connected to ADC0 channel 26
 *
 * Note: To print floating point numbers using KDS you must uncomment the line
 * "#define PRINTF_FLOAT_ENABLE   1" in the file print_scan.h in the utilities folder.
 */
float read_temperature()
{
	float temp, vtemp;
	vtemp = ((float)read_adc0(TEMPERATURE_CHANNEL) * 3.3)/65536;
	temp = 25 - ((vtemp - 0.716)/0.00162);
	return(temp);
}


