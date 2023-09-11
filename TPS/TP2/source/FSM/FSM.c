/***************************************************************************//**
  @file     FSM.c
  @brief    fsm functions.
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


#include "FSM.h"

#include "States/id_entry.h"
#include "States/encoder_entry.h"
#include "States/card_entry.h"
#include "States/pin_entry.h"
#include "States/open.h"
#include "States/brightness.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

//TODO: borrar esto, que hacen estos defines aca xd????? se rompera el programa si los borro?
#define IDLE 0
#define RISING_FLANK 2


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void pass(void);


/*******************************************************************************
 * FSM TABLE
 ******************************************************************************/

state BRIGHTNESS[]=
{
		{pass, ENC_PRESSED_EV, BRIGHTNESS},

		{incr_bri, ENC_RIGHT_EV, BRIGHTNESS},

		{decr_bri, ENC_LEFT_EV, BRIGHTNESS},

		{pass, END_TABLE, BRIGHTNESS}
};

state ID_ENTRY[]=
{
		{pass, NONE_EV, ID_ENTRY},

		{init_id, ENC_LEFT_EV, ENCODER_ENTRY},

		{init_id, ENC_RIGHT_EV, ENCODER_ENTRY},

		{init_id, ENC_PRESSED_EV, ENCODER_ENTRY},

		{init_cardswipe, CARD_SWIPE_EV, CARD_ENTRY},

		{init_failed_cardswipe, CARD_MIDSWIPE_EV, CARD_ENTRY},

		{bri_message, INCREASE_BRIGHTNESS_EV, BRIGHTNESS},

		{pass, END_TABLE, ENCODER_ENTRY}
};


state ENCODER_ENTRY[]=
{
		{welcome_animation, RETURN_EV, ID_ENTRY},

		{down_number, ENC_LEFT_EV, ENCODER_ENTRY},

		{up_number, ENC_RIGHT_EV, ENCODER_ENTRY},

		{accept_number, ENC_PRESSED_EV, ENCODER_ENTRY},

		{msg_fail_encoder, ID_FAIL_ENC_EV, RED_LED_ON},

		{msg_ok_encoder, ID_OK_ENC_EV, PIN_ENTRY},

		{bri_message, INCREASE_BRIGHTNESS_EV, BRIGHTNESS},

		{pass, END_TABLE, ENCODER_ENTRY},
};

state CARD_ENTRY[]=
{
		{msg_fail_card, ID_FAIL_CARD_EV, RED_LED_ON},

		{init_pin, ID_OK_CARD_EV, PIN_ENTRY},

		{msg_error_card, RETURN_EV, RED_LED_ON},

		{bri_message, INCREASE_BRIGHTNESS_EV, BRIGHTNESS},

		{pass, END_TABLE, CARD_ENTRY},

};

state PIN_ENTRY[]=
{
		{pin_down_number, ENC_LEFT_EV, PIN_ENTRY},

		{pin_up_number, ENC_RIGHT_EV, PIN_ENTRY},

		{pin_accept_number, ENC_PRESSED_EV, PIN_ENTRY},

		{msg_fail_pin, PIN_FAIL_EV, PIN_ENTRY},

		{msg_pin_3_times, PIN_3_TIMES_EV, RED_LED_ON},

		{msg_ok_pin, PIN_OK_EV, GREEN_LED_ON},

		{msg_pin_short, PIN_SHORT_EV, PIN_ENTRY},

		{welcome_animation, RETURN_EV, ID_ENTRY},

		{bri_message, INCREASE_BRIGHTNESS_EV, BRIGHTNESS},

		{pass, END_TABLE, PIN_ENTRY},

};

state RED_LED_ON[]=
{
		{welcome_animation, FIVE_SEC_LAPSE_EV, ID_ENTRY},

		{pass, NONE_EV, RED_LED_ON},

		{pass, END_TABLE, RED_LED_ON}

};

state GREEN_LED_ON[]=
{
		{welcome_animation, FIVE_SEC_LAPSE_EV, ID_ENTRY},

		{pass, END_TABLE, RED_LED_ON},

};


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/**
 * @brief determines next state and executes transition functions based on current event, current state and fsm table.
 * @param p_state: pointer to current state inside the fsm state table.
 * @param curr_event: latest event, it came from the event queue.
 * @return pointer to the next state following the logic of the state table.
 */
state* fsm_dispatcher(state* p_state, Event_Type curr_event)
{
	bool flag = 1;

	while(flag)
	{
		if(p_state->event == curr_event || p_state->event == END_TABLE)
		{
			flag=0;
			if (curr_event == INCREASE_BRIGHTNESS_EV)
			{
				BRIGHTNESS[0].next_state = p_state;
			}
		}
		else
		{
			p_state++;
		}
	}

	(*p_state->funct)();

	p_state = p_state->next_state;

	return p_state;
}


/**
 * @brief returns initial state of the fsm, gets called at the beginning of the program.
 * @return pointer to the initial state of the fsm.
 */
state* get_initial_state()
{
	return ID_ENTRY;
}


/**
 * @brief function executed at the beginning of the fsm.
 * @return nothing.
 */
void start_fsm()
{
	welcome_animation();
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/**
 * @brief dummy function.
 * @return nothing.
 */
static void pass(void)
{

}


/*******************************************************************************
 ******************************************************************************/
