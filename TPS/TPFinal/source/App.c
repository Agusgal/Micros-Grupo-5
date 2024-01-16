/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <FSM_1/FSM.h>
#include "MK64F12.h"
#include "hardware.h"

#include "Drivers/HAL/Encoder.h"
#include "board.h"
#include "Drivers/MCAL/gpio.h"

#include "AudioPlayer.h"

#include "EventQueue/queue.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


//TODO: que hace esto aca?????
void SysTick_Init(void);

/**
 * @brief captures events generated within the drivers and inside states and fills the eventqueue. Gets called continously.
 * @return nothing.
 */
void fill_queue(void);

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


//this variable saves the current state of the FSM
static state *current_state;


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	//SysTick_Init();
	//Timer_Init();

	//Memory Init
	//OLED Init
	//Matrix Init

	//MP3 decoder Init

	//Audio PLayer Init
	AudioPlayer_Init();

	//Vumeter/FFT Init
	//Equalizer Init

	//Daytime Init

	//Init Queue
	queue_Init();

	//Init Encoder
	Encoder_Init();

	//Init fsm
	current_state = get_initial_state();
	start_fsm();

	//TODO: cambiar
    hw_DisableInterrupts();
    SysTick_Init();
    hw_EnableInterrupts();
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


/**
 * @brief captures events generated within the drivers and inside states and fills the eventqueue. Gets called continously.
 * @return nothing.
 */
void fill_queue(void)
{

	//check for encoder events


	//Check for Encoder press events


	//Check for button events


	//Check for OLEd events


	//Check for AudioPLayer Events
	if (AudioPlayer_IsBackBufferFree())
	{
		push_Queue_Element(FILL_BUFFER_EV);
	}


	//Check for memory events
}
