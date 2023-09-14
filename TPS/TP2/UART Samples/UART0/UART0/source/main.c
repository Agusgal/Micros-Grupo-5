/*
 * main.c
 *
 * Simple UART test program
 *
 */

#include "hardware.h"
#include "UART.h"

#define __FOREVER__ 	for(;;)

int main (void)
{


unsigned char uart_data;

 	 	 	hw_Init ();
			UART_Init();

			UART_SendMsg("La concha de tu madre");


			// Enable interrupts
			hw_EnableInterrupts();

			__FOREVER__
			{
					uart_data=UART_Recieve_Data();
					UART_Send_Data(uart_data+1);
			}




}



