
/***********************************************************************************************
 * program to generate interrupt when switch is pushed
 *
 **********************************************************************************************/
#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "gpio.h"

int count = 0;
char ch;

int main(void)
{

    //Initialise the FRDM-KL26Z Board
	hardware_init();
	FRDM_KL26Z_LEDs_Configure();
	// configure sw1 for falling edge interrupt
	NVIC_ClearPendingIRQ(31);
	NVIC_EnableIRQ(31);
	FRDM_KL26Z_SW1_Configure(PULLUP,FALLING_EDGE);


    while(1) {

    }
    /* Never leave main */
    return 0;
}

void PORTC_PORTD_IRQHandler()
{
	PORTC_ISFR |= SW1_MASK; // clear interrupt flag
	if(count >=4)
		count = 0;
	switch (count)
	{
		case 0:
			LED_set(ALL,OFF);
			break;
		case 1:
			LED_set(BLUE,ON);
			break;
		case 2:
			LED_set(ALL,OFF);
			LED_set(RED,ON);
			break;
		case 3:
			LED_set(ALL,OFF);
			LED_set(GREEN,ON);
			break;
	}
	count ++;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
