/***************************************************************************//**
  @file     FSM.c
  @brief    fsm functions.
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


#include "FSM.h"
#include "States/Init.h"
#include "States/idle.h"


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

state INIT_STATE[] =
{
		{startProgram, START_EV, IDLE_STATE},
		{pass, END_TABLE, INIT_STATE}
};


state IDLE_STATE[] =
{
		//Buttons
		{Idle_OnUserInteraction, PP_EV, IDLE_STATE},
		{Idle_OnUserInteraction, NEXT_EV, IDLE_STATE},
		{Idle_OnUserInteraction, PREV_EV, IDLE_STATE},
		{Idle_OnUserInteraction, STOP_EV, IDLE_STATE},

		//Encoder
		{Idle_OnUserInteraction, ENCODER_PRESS_EV, IDLE_STATE},
		{Idle_OnUserInteraction, ENCODER_RIGHT_EV, IDLE_STATE},
		{Idle_OnUserInteraction, ENCODER_LEFT_EV, IDLE_STATE},

		//Misc
		//{FileSelection_InitState, START_EV, FILE_SELECT_STATE},
		{Idle_OnUserInteraction,  SD_IN_EV, IDLE_STATE},
		{pass, END_TABLE, INIT_STATE}
};



state FILE_SELECT_STATE[] =
{

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
			flag = 0;
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
	return IDLE_STATE;
}


/**
 * @brief function executed at the beginning of the fsm.
 * @return nothing.
 */
void start_fsm()
{
	//welcome_animation();
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
