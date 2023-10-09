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

	RXB_RAWDATA_t data = {0x105, 8, {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8}};

	CAN_SPI_SendInfo(&data);

	// Enable interrupts

	__FOREVER__
	{

	}

}




