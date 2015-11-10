
/***********************************************************************************************
 * program to generate interrupt when switch is pushed
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
	adc0_config(SW_TRIGGER,BIT16,ADC_INTERRUPT_ENABLED);
	PIT_Configure_interrupt_mode(2);
	PRINTF("~~~Starting ADC read~~~\r\n");
    while(1) {
    	/*int sample = read_adc0(ch);
    	PRINTF("\rSAMPLE IS: %04d",sample);
    	for(i = 0; i < 10000000; i ++);
    	for(i = 0; i < 10000000; i ++);
    	for(i = 0; i < 10000000; i ++);
    	for(i = 0; i < 10000000; i ++);
*/
    }
    /* Never leave main */
    return 0;

}
void ADC0_IRQHandler()
{
	int sample = read_adc0(ch);
	float vtemp = (float)sample/65536 *3.3;
	float temp = vtemp * 100; // 10 mV per degree so multiply by 100
	PRINTF("\rSAMPLE IS: %02d",(unsigned int)temp);
}
void PIT_IRQHandler()
{
	PIT_TFLG0 = 0x01ul;
	ADC0_SC1A = 0x4Fu;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
