/***********************************************************************************************
 *Author: Brian Cunnane
 *Date: 04/Apr/2016
 *Description: Weighing scales test code
 **********************************************************************************************/
#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "gpio.h"
#include "pit_kl26z.h"

/*****************************************************
 * Pseudocode description:
 * toggle clock pin 25 times
 * read data on datapin after each toggle
 * toggle clock once to reset
 * wait for data to go high before rereading
 */

int main(void)
{

	hardware_init();
	PRINTF("Weighing scales test code\r\n");

    while(1)
    {}

    return 0;
}
