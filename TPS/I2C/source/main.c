/*
 * main.c
 *
 */

#include "i2c.h"
#include "hardware.h"
#include <FXOS8700CQ.h>
#include "accel.h"

#define __FOREVER__ 	for(;;)

volatile int pausa=1;
volatile int pausa1=1;
volatile int pausa2=1;
volatile int pausa3=1;
volatile int pausa4=1;


int main (void)
{
	uint8_t read_buffer[20]={2,1,1,1,1,1};
	uint8_t write_buffer[]={1};

	I2C_InitModule(0);
	while(pausa);
	I2C_Read_FX_Reg(M_CTRL_REG1,read_buffer);

	while(pausa1);
	I2C_Read_FX_Reg(TEMP_REG,read_buffer);
	while(pausa2);
	I2C_Write_FX_Reg(M_CTRL_REG1,write_buffer);
	while(pausa3);
	I2C_Read_FX_Reg(TEMP_REG,write_buffer);

	while (pausa4);
}

/* COMENTARIOS
 *
 * Las pausas estan ahi porque como funcionan con la misma rutina de interrupción y el mismo I2C_Object, se pisan
 * y jajan't. Habría que implementarlo con la queue de Brunito. Leí los registros de WHO_AM_I, CTRL_REG1 y TEMP_REG y
 * otras cosas y parece andar joya.
 *	El I2C_OBject está para la implementación de las cosas en la CPU (compu) y el I2C_Module es posta el I2C0 del micro.
 *
 */

