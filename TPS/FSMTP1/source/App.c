/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


#include "Drivers/board.h"
#include "Drivers/gpio.h"
#include <stdio.h>
#include "MK64F12.h"
#include "hardware.h"
#include "Drivers/Display.h"
#include "Drivers/Encoder.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void idle(void);
void SysTick_Init (void);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{

	gpioMode(PIN_LED_RED,OUTPUT);
	//gpioMode(PIN_LED_GREEN,OUTPUT);
	//gpioMode(PIN_LED_BLUE,OUTPUT);

	gpioMode(PIN_SW3,INPUT);

	//gpioMode(PIN_SW2,INPUT_PULLUP);
	Display_Init();
	Encoder_Init();

    hw_DisableInterrupts();
    SysTick_Init();
    hw_EnableInterrupts();
    //NVIC_EnableIRQ(PORTC_IRQn);
    //gpioIRQconfig(PIN_SW2,PORT_eInterruptFalling);
    //gpioIRQconfig(PIN_CH_A,PORT_eInterruptEither);
    //gpioIRQconfig(PIN_CH_B,PORT_eInterruptEither);
    gpioWrite(PIN_LED_RED,!LED_ACTIVE);
	//gpioWrite(PIN_LED_GREEN,!LED_ACTIVE);
	//gpioWrite(PIN_LED_BLUE,!LED_ACTIVE);
    updateDisplay("00000");
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	static int counter=0;
	bool flank=gpioFlank(PIN_SW3,SW_ACTIVE);


	if (flank & (counter==0))
	{
		counter++;
		updateDisplay("HOlA BUEN DIA COMO ESTAS");
	}
	else if (flank & (counter==1))
	{
		counter++;
		updateDisplay("CONT_SCR_R");
	}
	else if (flank & (counter==2))
	{
		counter++;
		updateDisplay("PAUSE_SCR");
	}
	else if (flank & (counter==3))
		{
			counter++;
			updateDisplay("SCR_L");
		}
	else if (flank & (counter==4))
		{
			counter=0;
			updateDisplay("CONT_SCR_L");
		}

}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void idle(void)
{

}


/*******************************************************************************
 ******************************************************************************/
