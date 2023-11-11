/***************************************************************************//**
  @file     App.c
  @brief    Application functions
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
#include "Drivers/UART.h"



/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

int demodulado = 1;
long long contador = 0;


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

//Callback del demodulador
void demodulator_clb(void);



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

	//TODO: configure UART
	//UART_SetBaudRate();

	//TODO: Init timers


	//gpioMode(PIN_LED_BLUE, OUTPUT);
	//gpioMode(PIN_TP, OUTPUT);
	//gpioWrite (PIN_TP, false);

	//TODO: Init modulator
	Demodulator_Init(demodulator_clb);
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	//Codigo de prueba
	/*
	if (demodulado == 1)
	{
		if (contador < 100)
		{
			contador ++;
			gpioWrite(PIN_LED_BLUE, (contador % 5000 >= 5000/2) ? HIGH : LOW );
		}
		else
		{
			updateWord();
			gpioWrite(PIN_LED_BLUE, HIGH);
		}
	}
	else
	{
		gpioWrite(PIN_LED_BLUE, LOW);
	}
	*/

	if(isDataReady() == true)
	{
		demodulate();
	}
}


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

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



/*******************************************************************************
 ******************************************************************************/
