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
#include "Drivers/CardReader_DRV.h"

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

	//Init card_reader
	cardReader_Init();

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
	uint8_t card_var = getCardReader_Status();


	//check for encoder turn events
	int move_enc = getEncoder_State();

	if (move_enc == 1) //move right
	{
		push_Queue_Element(ENC_RIGHT_EV);
	}
	else if (move_enc == 2)
	{
		push_Queue_Element(ENC_LEFT_EV);
	}

	//Check for Encoder press events
	int encoder_state = getEncoderSwitch_State();
	if (encoder_state == RELEASED)
	{
		push_Queue_Element(ENC_PRESSED_EV);
	}
	else if (FIVE_SEC_PRESS)
	{
		push_Queue_Element(INCREASE_BRIGHTNESS_EV);
	}

	//Check for Card Events
	if (!card_var)
	{
		push_Queue_Element(CARD_SWIPE_EV);
	}
	else if (card_var == CARD_FAIL)
	{
		push_Queue_Element(CARD_MIDSWIPE_EV);
	}

	//Check for timer events (leds)
	if (get_green_status())
	{
		push_Queue_Element(FIVE_SEC_LAPSE_EV);
	}
}


/*******************************************************************************
 ******************************************************************************/
