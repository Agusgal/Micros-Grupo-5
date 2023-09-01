/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdio.h>



#include "MK64F12.h"
#include "hardware.h"

#include "Drivers/Display.h"
#include "Drivers/Encoder.h"
#include "Drivers/board.h"
#include "Drivers/gpio.h"
#include "Drivers/BoardLeds.h"

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
	queue_Init();

	//Init display
	Display_Init();

	//Init Leds
	BoardLeds_Init();

	//Init Timers

	//Init reader

	//Init Encoder
	Encoder_Init();

	//Init fsm
	current_state = get_initial_state();
	start_fsm();

    hw_DisableInterrupts();
    SysTick_Init();
    hw_EnableInterrupts();
    //writeMessage("8888",false);
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




	/* para debuggear el brightness
	 int enc=getEncoderSwitch_State();
	 if (enc==RISING_FLANK)
		incBrightness();

	if (getEncoderSwitch_State()==RELEASED)
	{
		pauseMessage();
	}
	*/

	//check for encoder turn events

	int move_enc = getEncoder_State();

	if(move_enc == 1) //move right
	{
		push_Queue_Element(ENC_RIGHT_EV);
	}

	else if (move_enc == 2)
	{
		push_Queue_Element(ENC_LEFT_EV);
	}

	//check for encoder press events
	if (getEncoderSwitch_State())
	{
		push_Queue_Element(ENC_PRESSED_EV);
	}




	//check for timer events.

	//etcetcetcect....
}


/*******************************************************************************
 ******************************************************************************/
