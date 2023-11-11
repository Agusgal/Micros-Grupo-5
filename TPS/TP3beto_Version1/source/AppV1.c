/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * EXPLICACION DE LOGICA DE PROGRAMA
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

int demodulado = 1;
long long contador = 0;



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

	//TODO: configure UART
	//UART_SetBaudRate();

	//TODO: Init timers

	//TODO: Init modulator
	Demodulator_Init(demodulator_clb);
}


/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	//Testing
	uint8_t a = 'h';
	modulator_sendChar(a);
	modulator_sendChar(0);
	modulator_sendChar(0);


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

	//Envio el mensjae por UART 0, TODO: chequear que canal usar
	UART_SendMsg(str, 0);

	demodulado = 1;
	contador = 0;
}

void modulator_clb(void)
{
	//debuging and measuring code (gpios para testpoints)
}


/*******************************************************************************
 ******************************************************************************/
