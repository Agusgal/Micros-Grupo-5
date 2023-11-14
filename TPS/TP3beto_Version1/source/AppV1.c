/***************************************************************************//**
  @file     App.c
  @brief    Application functions	(VERSIÓN 1)
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "MK64F12.h"
#include "hardware.h"
#include "Drivers/board.h"
#include "Drivers/gpio.h"
#include "Drivers/port.h"
#include "Drivers/DemodulatorV1.h"
#include "Drivers/ModuladorV1.h"
#include "Drivers/UART.h"


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

//Callback del demodulador
void demodulator_clb(void);

//Callback del modulador
void modulator_clb(void);


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/



/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	PORT_Init();
	UART_Init();

	gpioMode(TP_PIN, OUTPUT);
	gpioWrite(TP_PIN, LOW);

	Modulator_Init(modulator_clb);
	Demodulator_Init(demodulator_clb);
}


/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	//UART ---> FSK
	if (UART_Get_Status(0))
	{
		unsigned char data = UART_Get_Data(0);
		modulator_sendChar(data);
	}

//	for(int i = 0; i < 3; i++)
//	{
//		modulator_sendChar('a');
//		modulator_sendChar('b');
//		modulator_sendChar('c');
	//}

	//FSK ---> UART
	if(isDataReady() == true)
	{
		demodulate();
	}
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void demodulator_clb(void)
{
	//obtendo ultimo mensaje
	char b = get_Msg();


	char str[3];
	str[0] = b;
	str[1] = '\0';

	//Envio el mensaje por UART 0,
	//UART_Send(str, 0);
	UART_SendChar(b, 0);
}

void modulator_clb(void)
{
	//debuging and measuring code (gpios para testpoints)
}


/*******************************************************************************
 ******************************************************************************/
