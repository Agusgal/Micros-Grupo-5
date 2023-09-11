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

//TODO: habra una mejor manera de manejar esto?
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
	else if(encoder_state == FIVE_SEC_PRESS)
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
	else if (get_red_status())
	{
		push_Queue_Element(FIVE_SEC_LAPSE_EV);
	}
}


/*******************************************************************************
 ******************************************************************************/
