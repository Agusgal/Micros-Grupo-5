/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdio.h>

#include "Drivers/board.h"
#include "Drivers/gpio.h"

#include "MK64F12.h"
#include "hardware.h"
#include "Drivers/Display.h"
#include "Drivers/Encoder.h"

#include "EventQueue/queue.h"
#include "FSM/FSM.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void idle(void);
void SysTick_Init (void);

//Function that fills the event queue with different events, gets called continously by app_run
void fill_queue(void);


static state *current_state;


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	//Init Queue

	//Init display
	Display_Init();

	//Init Leds

	//Init Timers

	//Init reader

	//Init Encoder
	Encoder_Init();

	//Init fsm
	current_state = get_initial_state();
	start_fsm();


	gpioMode(PIN_LED_RED,OUTPUT);
	//gpioMode(PIN_LED_GREEN,OUTPUT);
	//gpioMode(PIN_LED_BLUE,OUTPUT);

	//gpioMode(PIN_SW3,INPUT);

	//gpioMode(PIN_SW2,INPUT_PULLUP);

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
}



/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	fill_queue();
	Event_Type event = pull_Queue_Element();

	if (event != NONE_EV)
	{
		current_state = fsm_dispatcher(current_state, event);
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

void fill_queue(void)
{
	//check for Card events

	//check for encoder turn events

	//check for encoder press events

	//check for timer events.

	//etcetcetcect....
}


/*******************************************************************************
 ******************************************************************************/
