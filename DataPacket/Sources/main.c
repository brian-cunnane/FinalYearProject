/********************************************
 * Author: Brian Cunnane
 * Date: 16/Feb/2016
 * Description: Combining the ADC read and Uart Tx programs previously
 * written and adding data packet encapsulation
 *******************************************/
#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "adc.h"
#include "pit_kl26z.h"
#include "gpio.h"
#include "WeightSensor.h"

#define RDRF_MASK 0x20	//Receive Data Register Full Flag Mask
#define RIE_MASK 0x20	//Receive Interrupt Enable Mask
#define UART_S1_TRDE_MASK 0X80
#define CONVERSIONFACTOR 20

void UART1_config();
void put_char(char c);
void enable_UART1_receive_interrupt();
char* createPacket(char * element0);
char char_received();

char channel = 15;
char dataPacket[7] = {0};
char* masterPointer;
char* element0;
unsigned long int zero_offset = 0;
/*****************************
 * [0]Header
 * [1]Identifier
 * [2]Length
 * [3]Sensor1
 * [4]Sensor2
 * [5]Sensor3
 * [6]Checksum
 ****************************/

int main(void)
{
	int i = 0;
	hardware_init();
	UART1_config();
	enable_UART1_receive_interrupt();
	adc0_config(SW_TRIGGER,BIT16,ADC_INTERRUPT_DISABLED);
	PIT_Configure_interrupt_mode(20); // 2 second interrupt
	FRDM_KL26Z_CLK_Configure(); //configure PTC8 as output for clocking hx711
	FRDM_KL26Z_DATA_Configure(0, 0); //configure PTC9 as input for HX711

	masterPointer = dataPacket;
	element0 = masterPointer;
	calibrate(&zero_offset);
	while(1){

	}

    return 0;
}

/*********************************************
void ADC0_IRQHandler()
{
	int sample = ADC0_RA;
	float vtemp = (float)sample/65536*3.3;
	float temp = vtemp *100; // 10 mV per degree
	//send via uart1
}
*********************************************/

void PIT_IRQHandler()
{
	PIT_TFLG0 = 0x01ul;
	ADC0_SC1A = 0x04Fu;
	ADC0_SC1A &= 0xFFFFFFE0;
	ADC0_SC1A |= channel;

	createPacket(element0);


	int i;
	for(i = 0; i < 7; i ++)
	{
		put_char(dataPacket[i]);
	}
}

void UART1_config()
{
	int SBR; //serial Baud Rate
	//config uart1 pins
	PORTE_PCR0 |= 0x03u << 8; // mux ALT2 UART1_RX
	PORTE_PCR1 |= 0x03u << 8; // mux ALT2 UART1_TX
	//config baudrate
	SIM_SCGC4 |= 0x01u<<11; // enable clock
	UART1_C2&=0XF3;//disabling transmission and receiving until config complete
	SIM_SOPT2 |= 0x01u <<26;



	//osr is fixed @ 16 for uart2
	SBR = 78;//uart_clock/(16*9600);

	UART1_BDL = SBR & 0xff; //baudrate register low
	UART1_BDH |=((SBR & 0xff00)>>8);

	UART1_C1 = 0; //8 bit no parity
	UART1_C2 |= 0x0c; // enable tx & rx
	UART1_C3 = 0; // no special interrupts
}

void UART1_IRQHandler() // for chars received
{
	//if (UART1_S1 & RDRF_MASK)
	//{
		//put_char(UART1_D); // echo character

	//}
}

void put_char(char c)
{
	while((UART1_S1 & UART_S1_TDRE_MASK) == 0)//brackets to enforce order of presidence
	{}
	UART1_D = c;

}

void enable_UART1_receive_interrupt()
{
	NVIC_ClearPendingIRQ(13);
	NVIC_EnableIRQ(13);
	UART1_C2 |= RIE_MASK;
}

char* createPacket(char* element0)
{
	int i;
	int x;
	unsigned long int averageValue = 0;
	unsigned long int grams = 0;
	char checksum = 0x00;
	*element0 = 0x55; //assign header value
	element0 ++;
	//assign identifier
	*element0 = 0x01;
	element0 ++;
	//assign length
	*element0 = 0x04;
	element0 ++;
	//assign readings

	int sample = read_adc0(channel);
	float vtemp = (float)sample/65536 * 3.3;
	float temp = vtemp * 100;
	*element0  = (char)temp;

	//*element0 = 0x06; //s1
	element0++;
	averageValue = readAverageValue(averageValue, zero_offset);
	grams = averageValue/CONVERSIONFACTOR;
	PRINTF("\r\nzero = %ld\r\n", zero_offset);
	PRINTF("Weight: %ld\r\n",grams);
	*element0 = (char)grams;//s2
	element0++;
	*element0 = 0x08;//s3
	element0++;
	for(i = 0; i < 4; i ++)
	{
		checksum = checksum ^ (*(dataPacket + i + 2));//should be 0x0d for this test
	}
	*element0 = checksum;
	element0 = masterPointer; //return to start of array


}
