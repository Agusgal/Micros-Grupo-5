/*
 * main.c
 *
 *  Created on: May 1, 2015
 *      Author: Juan Pablo VEGA - Laboratorio de Microprocesadores
 */

#include "hardware.h"
#include "UART.h"
#include "SPI.h"
#include "CAN_SPI.h"

#define __FOREVER__ 	for(;;)

int main (void)
{
	unsigned char uart_data;

	hw_DisableInterrupts();
	hw_Init ();
	UART_Init();
	hw_EnableInterrupts();
	CAN_SPI_Init();


	while(SPI_Transmission_In_Process());
	SPI_SendData("a",1, 0);
	while(SPI_Transmission_In_Process());
	SPI_SendMsg("Hola");
	while(SPI_Transmission_In_Process());
	SPI_SendMsg("Messi");


	// Enable interrupts

	__FOREVER__
	{

	}

}




