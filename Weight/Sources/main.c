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
	long int sample = 0;
	hardware_init();
	FRDM_KL26Z_CLK_Configure();//configure PTC8 as output for clocking hx711
	FRDM_KL26Z_DATA_Configure(0,0);//configure PTC9 as input for HX711
	PRINTF("Weighing scales test code\r\n");
	char data[24] = {0};
    while(1)
    {
    	int x;
    	int i;

    	GPIOC_PCOR |= CLK_MASK; //PTC8 = 0
    	while(DATA_read());
    	for(x = 0; x < 24; x++)
    	{
    		//GPIOC_PCOR |= CLK_MASK; //PTC8 = 0
    		GPIOC_PSOR |= CLK_MASK; //PTC8 = 1
    		for(i = 0; i < 200; i ++); //delay
    		//GPIOC_PCOR |= CLK_MASK;
    		//for(i = 0; i < 200; i ++);
    		//while(DATA_read());
    		if(DATA_read())
    		{
    			data[x] = 0x01u;//read data into char array to see binary value
    			sample |= 0x01u;//read data into long int.
    		}
    		sample <<= 1;
    		GPIOC_PCOR |= CLK_MASK;
    		for(i = 0; i < 200; i ++);

    	}
    	sample >>= 1;
    	GPIOC_PSOR |= CLK_MASK;
    	for(i = 0; i < 200; i ++);
    	GPIOC_PCOR |= CLK_MASK;
    	//sample ^= 0x800000; convert from 2s compliment
    	/*PRINTF("SAMPLE is %ld\r\n",sample);
    	int sign = sample >> 23;
    	PRINTF("sign: %d\r\n",sign);
    	if(sign == 1){
    		PRINTF("IT'S TWO'S COMPLIMENT\r\n");
    		unsigned long int value = (sample + sign) ^ sign;
    		PRINTF("Value is %lu\r\n",value);
    	}
    	*/
    	for(x = 0; x < 24; x ++)
    	{
    		PRINTF("%d", data[x]);
    		data[x] = 0x00u;
    	}
    	PRINTF("\r\nSample is %ld \r\n",sample);
    	sample = 0;
    }

    return 0;
}
