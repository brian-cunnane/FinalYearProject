/*
 * adc.h
 *
 * Header file for KL26Z ADC
 *
 *  Created on: 14 Oct 2015
 *      Author: nokeeffe
 */

#ifndef ADC_ADC_H_
#define ADC_ADC_H_

//ADC Channels
#define TEMPERATURE_CHANNEL 26
#define LIGHT_SENSOR_CHANNEL 3
#define POT_CHANNEL 8

//ADC interrupt configuration
#define ADC_INTERRUPT_DISABLED 0
#define ADC_INTERRUPT_ENABLED 1

//ADC Modes
#define BIT8 0
#define BIT12 1
#define BIT10 2
#define BIT16 3

//Trigger Modes
#define SW_TRIGGER_MASK 0xBF
#define HW_TRIGGER_MASK 0x40
#define SW_TRIGGER 0
#define HW_TRIGGER 1

//function Prototypes
unsigned int read_adc0(char channel_no);
void adc0_config(char trigger, char mode, char interrupt_config);
float read_temperature();



#endif /* ADC_ADC_H_ */
