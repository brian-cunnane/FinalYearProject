
/***********************************************************************************************
 * program to generate interrupt when switch is pushed
 *
 **********************************************************************************************/
#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"

char ch;

int main(void)
{

    //Initialise the FRDM-KL26Z Board
	hardware_init();

	//Print message to serial terminal
	PRINTF("First Embedded Systems Lab\r\n");
	PRINTF("Type a character and it will be echoed back\r\n\n");


    while(1) {
    	// Main routine that simply echoes received characters forever

		// First, get character
		ch = GETCHAR();

		// Now echo the received character
		PUTCHAR(ch);
    }
    /* Never leave main */
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
