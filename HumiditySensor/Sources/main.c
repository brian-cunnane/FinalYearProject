/*
 *Author: Brian Cunnane
 *Date: 23/Mar/2016
 *Description: ADC read of Humidity Sensor
 */

#include "adc.h"
#include "pit_kl26z.h"
#include "board.h"
#include "fsl_clock_manager.h"
#include "fsl_debug_console.h"
#include <stdlib.h>


#define HUMIDITYCHANNEL 14

int main(void)
{
	hardware_init();
	adc0_config(SW_TRIGGER,BIT16,ADC_INTERRUPT_DISABLED);
	PIT_Configure_interrupt_mode(1);
	PRINTF("~~~Starting ADC read~~~\r\n");
	while(1){}

	return 0;
}

void PIT_IRQHandler()
{
	PIT_TFLG0 = 0x01ul;
	ADC0_SC1A = 0x04Fu;
	ADC0_SC1A &= 0xFFFFFFE0;
	ADC0_SC1A |= HUMIDITYCHANNEL;
	int sample = read_adc0(HUMIDITYCHANNEL);
	PRINTF("Sample is %d\r\n",sample);
}
