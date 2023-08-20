/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "board.h"
#include "gpio.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void delayLoop(uint32_t veces);


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
    gpioMode(PIN_LED_GREEN, OUTPUT);
    gpioMode(PIN_SW3, SW_INPUT_TYPE);

}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	// se cambia de estado cuando se presiona el botón
	if (!gpioRead(PIN_SW3))
	{
		delayLoop(5000UL);	// 5 ms para chequear el estado del botón
		if (!gpioRead(PIN_SW3))
		{
			gpioToggle(PIN_LED_GREEN);
		}
	}
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void delayLoop(uint32_t veces)
{
	/*
	 * Si tarda 7 instrucciones, entonces hay que dividir por 7 al número recibido
	 */
	veces = veces * 100 / 7;
    while (veces--); //(el primer nop no se repite) + 2 + 1 + 2 + 1 + 1 = 7
}


/*******************************************************************************
 ******************************************************************************/
