/*
 * main.c
 *
 *  Created on: May 1, 2015
 *      Author: Juan Pablo VEGA - Laboratorio de Microprocesadores
 */

#include "hardware.h"
#include "UART.h"
#include "SPI.h"

#define __FOREVER__ 	for(;;)

int main (void)
{
	unsigned char uart_data;

	hw_DisableInterrupts();
	hw_Init ();
	UART_Init();
	SPI_Init();



	hw_EnableInterrupts();
	SPI_SendMsg("Hola");
	SPI_SendMsg("Messi");

	// Enable interrupts

	__FOREVER__
	{

	}

}




