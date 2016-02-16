/********************************************
 * Author: Brian Cunnane
 * Date: 16/Feb/2016
 * Description: Combining the ADC read and Uart Tx programs previously
 * written and adding data packet encapsulation
 */
#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "adc.h"
#include "pit_kl26z.h"
#include "gpio.h"

#define RDRF_MASK 0x20 // UART received data register flag mask
#define RIE_MASK 0x20 // UART receive interrupt enable mask
//#define UART_S1_TDRE_MASK 0x80

void UART1_config();
void put_char(char c);
void enable_UART1_receive_interrupt();

char channel = 15;

int main(void)
{
	int i = 0;
	hardware_init();
	UART1_config();
	enable_UART1_receive_interrupt();
	adc0_config(SW_TRIGGER,BIT16,ADC_INTERRUPT_ENABLED);
	PIT_Configure_interrupt_mode(2); // 2 second interrupt

	while(1){}

    return 0;
}

void ADC0_IRQHandler()
{
	int sample = ADC0_RA;
	float vtemp = (float)sample/65536*3.3;
	float temp = vtemp *100; // 10 mV per degree
	//send via uart1
}

void PIT_IRQHandler()
{
	PIT_TFLG0 = 0x01ul;
	ADC0_SC1A = 0x04Fu;
	ADC0_SC1A &= 0xFFFFFFE0;
	ADC0_SC1A |= channel;
}

void UART1_config()
{
	int SBR; //serial Baud Rate
	//enable port 2 & uart 1 clocks
	//SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
	SIM_SCGC4 |= 0x01u<<11; // enable clock
	SIM_SOPT2 |= 0x01u <<26;

	//config uart1 pins
	PORTE_PCR0 |= 0x03u << 8; // mux ALT2 UART1_RX
	PORTE_PCR1 |= 0x03u << 8; // mux ALT2 UART1_TX

	//config baudrate

	//osr is fixed @ 16 for uart2
	SBR = 78;//uart_clock/(16*9600);

	UART1_BDL = SBR && 0xff; //baudrate register low
	UART1_BDH |=((SBR & 0xff00)>>8);

	UART1_C1 = 0; //8 bit no parity
	UART1_C2 |= 0x0c; // enable tx & rx
	UART1_C3 = 0; // no special interrupts
}

void UART1_IRQHandler()
{
	if (UART1_S1 & RDRF_MASK)
	{
		put_char(UART1_D); // echo character
	}
}

void put_char(char c)
{
	if(UART1_S1 & UART_S1_TDRE_MASK ==0)
	{}
	UART1_D = c;
}

void enable_UART1_receive_interrupt()
{
	NVIC_ClearPendingIRQ(13);
	NVIC_EnableIRQ(13);
	UART1_C2 |= RIE_MASK;
}
