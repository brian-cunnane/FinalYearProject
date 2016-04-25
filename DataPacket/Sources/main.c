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
#include "TemperatureSensor.h"
#include "WeightSensor.h"
#include "HumiditySensor.h"
#include "Accelerometer.h"

#define RDRF_MASK 0x20	//Receive Data Register Full Flag Mask
#define RIE_MASK 0x20	//Receive Interrupt Enable Mask
#define UART_S1_TRDE_MASK 0X80
#define CONVERSIONFACTOR 20


void UART1_config();
void put_char(char c);
void enable_UART1_receive_interrupt();
char* createPacket(char * element0);
char char_received();


char dataPacket[11] = {0};
char* masterPointer;
char* element0;
unsigned long int zero_offset = 0;
/*****************************
 * [0]Header
 * [1]Identifier
 * [2]Length
 * [3]Temperature
 * [4]Weight
 * 	[5]
 * 	[6]
 * 	[7]
 * [8]Humidity
 * [9]Accelerometer
 * [10]Checksum
 ****************************/

int main(void)
{
	int i = 0;
	unsigned long int *ptr;
	ptr = &zero_offset;
	hardware_init();
	UART1_config();
	enable_UART1_receive_interrupt();
	adc0_config(SW_TRIGGER,BIT16,ADC_INTERRUPT_DISABLED);
	PIT_Configure_interrupt_mode(20); // 2 second interrupt
	configureAccelerometer();
	FRDM_KL26Z_CLK_Configure(); //configure PTC8 as output for clocking hx711
	FRDM_KL26Z_DATA_Configure(0, 0); //configure PTC9 as input for HX711

	masterPointer = dataPacket;
	element0 = masterPointer;
	PRINTF("\r\nSTARTING\r\n");
	zero_offset = calibrate(ptr);
	PRINTF("\r\nCALIBRATION COMPLETE!\r\n");
	while(1){}

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
	//ADC0_SC1A |= TEMPERATURECHANNEL;

	createPacket(element0);


	int i;
	for(i = 0; i < 11; i ++)
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
	unsigned long int averageWeightValue = 0;
	unsigned long int grams = 0;
	float temp = 0;
	char checksum = 0x00;

	*element0 = 0x55; //assign header value
	element0 ++;
	//assign identifier
	*element0 = 0x01;
	element0 ++;
	//assign length
	*element0 = 0x0B;
	element0 ++;
	//assign readings
	temp = readTemperature();
	*element0  = (char)temp; //s1 Temperature
	element0++;

	averageWeightValue = readAverageValue(averageWeightValue, zero_offset);
	grams = averageWeightValue/CONVERSIONFACTOR;
	PRINTF("\r\nzero = %ld\r\n", zero_offset);
	PRINTF("Weight: %ld\r\n",grams);
	//unsigned long int needs to be split over 4 chars due to size
	*element0 = (grams >>24) & 0xFF;//s2
	element0++;
	*element0 = (grams >>16) & 0xFF;//s2
	element0++;
	*element0 = (grams >> 8) & 0xFF;//s2
	element0++;
	*element0 = grams & 0xFF;//s2
	element0++;

	*element0 = readHumidity(temp);//s3 Humidity
	element0++;

	*element0 =  readAccelerometer();
	element0++;
	for(i = 0; i < 8; i ++)
	{
		checksum = checksum ^ (*(dataPacket + i + 2));//should be 0x0d for this test
	}
	*element0 = checksum;
	element0 = masterPointer; //return to start of array
	for(i = 0; i < 11; i ++)
		PRINTF("%d ",dataPacket[i]);
	PRINTF("\r\n%x",checksum);

}
