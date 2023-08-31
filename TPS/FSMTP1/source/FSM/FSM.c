/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


#include "FSM.h"

#include "States/id_entry.h"
#include "States/encoder_entry.h"



#include <stdio.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define IDLE 0
#define RISING_FLANK 2


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void pass(void);


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/





/**
 * @brief
 * @param
 * @param
 */

state* fsm_dispatcher (state* p_state, Event_Type curr_event)
{
	bool flag = 1;

	while(flag)
	{
		if(p_state->event == curr_event && p_state->event != END_TABLE)
		{
			flag=0;
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


state* get_initial_state()
{
	return ID_ENTRY;
}

void start_fsm()
{
	welcome_animation();
}



state ID_ENTRY[]=
{
		{pass, NONE_EV, ID_ENTRY},

		{init_id, ENC_LEFT_EV, ENCODER_ENTRY},

		{init_id, ENC_RIGHT_EV, ENCODER_ENTRY},

		{init_id, ENC_PRESSED_EV, ENCODER_ENTRY},

		{init_id, CARD_SWIPE_EV, CARD_ENTRY}
};


state ENCODER_ENTRY[]=
{
		{up_number, ENC_LEFT_EV, ENCODER_ENTRY},

		{down_number, ENC_RIGHT_EV, ENCODER_ENTRY},

		{ok_number_encoder, ENC_PRESSED_EV, ENCODER_ENTRY},

		{msg_fail_encoder, ID_FAIL_ENC_EV, RED_LED_ON},

		{msg_ok_encoder, ID_OK_ENC_EV, PIN_ENTRY}

};

state CARD_ENTRY[]=
{
		//{msg_fail, ID_FAIL_CARD_EV, RED_LED_ON},

		//{msg_ok, ID_OK_CARD_EV, PIN_ENTRY}

};

state PIN_ENTRY[]=
{
		//{msg_fail,PIN_FAIL_EV, RED_LED_ON},

		//{msg_ok, PIN_OK_EV, GREEN_LED_ON}

};

state RED_LED_ON[]=
{
		//{five_sec_red, FIVE_SEC_LAPSE_EV, ID_ENTRY},

		//{one_min_red, MIN_LAPSE_EV, WRONG_ID}

};

state GREEN_LED_ON[]=
{
		//{five_sec_green, FIVE_SEC_LAPSE_EV, ID_ENTRY},

};

state WRONG_ID[]=
{
		//{five_sec_green, END_TABLE, ID_ENTRY},

};


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



static void pass(void)
{

}
/*******************************************************************************
 ******************************************************************************/
