/*
 * main.c
 *
 *  Created on: 18 Jan 2016
 *      Author: brian
 *      description: Program to output 0x55 on UART1 of KL26Z board for uart testing.
 */
#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"

void UART1_config();
void put_char(char c);
void tx_string(char *ptr);


int main(){
	int x;
	hardware_init();
	//tx_string("Serial example\r\n");
	UART1_config();
	//enable_UART2_receive_interrupt();
	NVIC_EnableIRQ(13);
	while(1)
	{
		put_char(0x55);
		for(x=0;x<10000;x++);
	}
}
void UART1_config(){
	long int uart_clock, br;
	unsigned int sbr, osr;
	unsigned char ch;

	//hardware_init();
	//enable port d & uart 2 clocks
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
	SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;

	//config clocks
	//BOARD_ClockInit();
	//config uart1 pins
	PORTE_PCR0 |= 0x03u << 8;//uart2tx/rx=1, tmp0_ch2/3=0,spio_miso/mosi = 0
	PORTE_PCR1 |= 0x03u << 8;

//config baudrate
	//uart_clock = CLOCK_SYS_GetPllFllClockFreq();
	//osr is fixed @ 16 for uart2
	sbr = 78;//uart_clock/(16*9600);
	//UART2_C4 = osr;
	UART1_BDL = sbr && 0xff; //baudrate register low
	UART1_BDH |=((sbr & 0xff00)>>8);

	UART1_C1 = 0; //8 bit no parity
	UART1_C2 |= 0x0c; // enable tx & rx
	UART1_C3 = 0; // no special interrupts
}

void UART1_IRQHandler(){
	if(UART1_S1 & UART_S1_RDRF_MASK){
		put_char(UART1_D); //echo received character
	}
}

void put_char(char c){
	while(UART1_S1 & UART_S1_TDRE_MASK == 0) //wait for empty tx buffer
	{}
	UART1_D = c;
}

void tx_string(char *ptr){
	while(*ptr != '\0')
		put_char(*ptr++);
}

