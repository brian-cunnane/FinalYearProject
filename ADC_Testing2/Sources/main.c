
/***********************************************************************************************
 * program to read temp from external sensor
 *
 **********************************************************************************************/
#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "adc.h"
#include "pit_kl26z.h"
#include "gpio.h"

char ch = 15;

int main(void)
{
	int i = 0;
    //Initialise the FRDM-KL26Z Board
	hardware_init();
	adc0_config(SW_TRIGGER,BIT16,ADC_INTERRUPT_DISABLED);
	PIT_Configure_interrupt_mode(2);// 2 second interrupt
	PRINTF("~~~Starting ADC read~~~\r\n");
    while(1) {

    }
    /* Never leave main */
    return 0;

}

/*
void ADC0_IRQHandler()
{
	//int sample = read_adc0(ch);
	int sample = ADC0_RA;
	PRINTF("\n%d\n", sample);
	float vtemp = (float)sample/65536 *3.3;
	float temp = vtemp * 100; // 10 mV per degree so multiply by 100
	PRINTF("\rSAMPLE IS: %02d",(unsigned int)temp);
}
*/

void PIT_IRQHandler()
{
	PIT_TFLG0 = 0x01ul;
	ADC0_SC1A = 0x4Fu;
	ADC0_SC1A &= 0xFFFFFFE0;
	ADC0_SC1A |= ch;//15

	int sample = read_adc0(ch);
	PRINTF("\n %d", sample);
	float vtemp = (float)sample/65536 * 3.3;
	float temp = vtemp * 100;
	PRINTF("\r %02d",(unsigned int)temp);
	sample = 0;

}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
