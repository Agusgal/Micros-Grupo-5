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
#include <stdio.h>
#include "MK64F12.h"
#include "hardware.h"
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
	gpioMode(PIN_SEG_A,OUTPUT_PULLDOWN);
	gpioMode(PIN_SEG_B,OUTPUT_PULLDOWN);
	gpioMode(PIN_SEG_C,OUTPUT_PULLDOWN);
	gpioMode(PIN_SEG_D,OUTPUT_PULLDOWN);
	gpioMode(PIN_SEG_E,OUTPUT_PULLDOWN);
	gpioMode(PIN_SEG_F,OUTPUT_PULLDOWN);
	gpioMode(PIN_SEG_G,OUTPUT_PULLDOWN);
	gpioMode(PIN_SEG_DP,OUTPUT);
	gpioMode(PIN_SEL0,OUTPUT);
	gpioMode(PIN_SEL1,OUTPUT);
	gpioMode(PIN_SW3,INPUT);
	gpioMode(PIN_CH_A,INPUT);
	gpioMode(PIN_CH_B,INPUT);
	gpioMode(PIN_SW2,INPUT_PULLUP);
	writeDigit(-1,0);
	writeDigit(-1,1);
	writeDigit(-1,2);
	writeDigit(-1,3);
    hw_DisableInterrupts();
    SysTick_Init();
    hw_EnableInterrupts();
    NVIC_EnableIRQ(PORTC_IRQn);
    gpioIRQconfig(PIN_SW3,PORT_eInterruptFalling);
    gpioIRQconfig(PIN_CH_A,PORT_eInterruptEither);
    gpioIRQconfig(PIN_CH_B,PORT_eInterruptRising);
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{


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
