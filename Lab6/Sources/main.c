
/***********************************************************************************************
 * program to generate interrupt when switch is pushed
 *
 **********************************************************************************************/
#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "gpio.h"
#include "FXOS8700.h"
#include "i2c.h"
#include "pit_kl26z.h"

int count = 0;
char ch;
char temp;
int x = 0;
int y = 0;
int z = 0;
char data[6] = {0x00};

int main(void)
{

    //Initialise the FRDM-KL26Z Board
	hardware_init();
	i2c0_configure();
	PIT_Configure_interrupt_mode(1);
	write_FXOS8700(CTRL_REG1_reg, 0x00);//put accelerometer into standby mode
	write_FXOS8700(XYZ_DATA_CFG_reg, 0x01);//set tolerance to +- 4g
	write_FXOS8700(CTRL_REG1_reg, 0x0D);//put accelerometer into active mode

	ch = read_FXOS8700(WHO_AM_I_reg);//read to check setup correctly
	PRINTF("\r\nWHO_AM_I: %x\r\n",ch);
	temp = read_FXOS8700(TEMPERATURE_reg);
	PRINTF("TEMPERATURE: %x\r\n",temp);// read to check setup correctly
    while(1) {


    }
    /* Never leave main */
    return 0;
}

void PIT_IRQHandler(){
	PIT_TFLG0=0x01ul;//clear interrupt flag

	for(int i = 0; i < 6; i ++)
		data[i] = read_FXOS8700(OUT_X_MSB_reg + i); //read all axes 8 bit so no need for lsb reg
	x = (data[0]<<6)+(data[1]>>2);//<<6 for msb
	y = (data[2]<<6)+(data[3]>>2);
	z = (data[4]<<6)+(data[5]>>2);

	PRINTF("X = %4d\r\n",x);
	PRINTF("Y = %4d\r\n",y);
	PRINTF("Z = %4d\r\n",z);
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
