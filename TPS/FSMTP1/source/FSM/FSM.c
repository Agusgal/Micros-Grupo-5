/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Bruno Di Sanzo
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "FSM.h"



/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


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

state* fsm_dispatcher (state* p_state, uint8_t curr_event)
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




state ID_ENTRY[]=
{
		{waiting_id,NONE_EV,ID_ENTRY},

		{init_id,ENC_LEFT_EV,ENCODER_ENTRY},

		{init_id,ENC_RIGHT_EV,ENCODER_ENTRY},

		{init_id,ENC_PRESSED_EV,ENCODER_ENTRY},

		{init_id,CARD_SWIPE_EV,CARD_ENTRY}
};


state ENCODER_ENTRY[]=
{
		{up_number,ENC_LEFT_EV,ENCODER_ENTRY},

		{down_number,ENC_RIGHT_EV,ENCODER_ENTRY},

		{ok_number,ENC_PRESSED_EV,ENCODER_ENTRY},

		{msg_fail,ID_FAIL_ENC_EV,RED_LED_ON},

		{msg_ok,ID_OK_ENC_EV,PIN_ENTRY}

};

state CARD_ENTRY[]=
{
		{msg_fail,ID_FAIL_CARD_EV,RED_LED_ON},

		{msg_ok, ID_OK_CARD_EV,PIN_ENTRY}

};

state PIN_ENTRY[]=
{
		{msg_fail,PIN_FAIL_EV,RED_LED_ON},

		{msg_ok, PIN_OK_EV,GREEN_LED_ON}

};

state RED_LED_ON[]=
{
		{five_sec_red, FIVE_SEC_LAPSE_EV,ID_ENTRY},

		{one_min_red, MIN_LAPSE_EV,WRONG_ID}

};

state GREEN_LED_ON[]=
{
		{five_sec_green, FIVE_SEC_LAPSE_EV,ID_ENTRY},

};

state WRONG_ID[]=
{
		{five_sec_green, END_TABLE,ID_ENTRY},

};




/*void fsm (bool timer_sec, bool timer_min)
{
	switch(states)
	{
	case ID_ENTRY:

		if(encoder) //aca habria que llamar a alguna funcion que devuelva que se esta ingresando la contra
			states = ENCODER_ENTRY;
		else if (card)
			states = CARD_ENTRY;

		break;

	case ENCODER_ENTRY:
			if(id_encoder == ENCODER)//hay que ver como se hace esto, y como es la base de datos
				states = PIN_ENTRY;
			else
				states = RED_LED_ON;
		break;

	case CARD_ENTRY:
			if(id_card == CARD)//hay que ver como se hace esto, y como es la base de datos
				states = PIN_ENTRY;

			else
				states = RED_LED_ON;
		break;

	case PIN_ENTRY:
			if()
		break;

	case RED_LED_ON:

			break;

	case GREEN_LED_ON:

			break;

	case BRIGHTNESS:

			break;

	case WRONG_ID:

			break;

	default:
		break;
	}

}

*/

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/




/*******************************************************************************
 ******************************************************************************/