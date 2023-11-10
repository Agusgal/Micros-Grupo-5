/***************************************************************************//**
  @file     FSM.c
  @brief    fsm functions.
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


#include "FSM.h"



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

		{pass, END_TABLE, BRIGHTNESS}
};

state ID_ENTRY[]=
{
		{pass, NONE_EV, ID_ENTRY},

		{pass, END_TABLE, ENCODER_ENTRY}
};


state ENCODER_ENTRY[]=
{

		{pass, END_TABLE, ENCODER_ENTRY},
};

state CARD_ENTRY[]=
{


		{pass, END_TABLE, CARD_ENTRY},

};

state PIN_ENTRY[]=
{


		{pass, END_TABLE, PIN_ENTRY},

};

state RED_LED_ON[]=
{

		{pass, NONE_EV, RED_LED_ON},

		{pass, END_TABLE, RED_LED_ON}

};

state GREEN_LED_ON[]=
{

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
