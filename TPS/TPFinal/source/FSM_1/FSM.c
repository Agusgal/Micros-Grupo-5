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
#include "States/file_selection.h"


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
		{Idle_OnUserInteraction, PLAYPAUSE_EV, IDLE_STATE},
		{Idle_OnUserInteraction, NEXT_EV, IDLE_STATE},
		{Idle_OnUserInteraction, PREV_EV, IDLE_STATE},
		{Idle_OnUserInteraction, STOP_EV, IDLE_STATE},

		//Encoder
		{Idle_OnUserInteraction, ENCODER_PRESS_EV, IDLE_STATE},
		{Idle_OnUserInteraction, ENCODER_RIGHT_EV, IDLE_STATE},
		{Idle_OnUserInteraction, ENCODER_LEFT_EV, IDLE_STATE},

		//Misc
		{FileSelection_InitState, START_EV, FILE_SELECT_STATE},
		{Idle_OnUserInteraction,  SD_IN_EV, IDLE_STATE},
		{pass, END_TABLE, IDLE_STATE}
};



state FILE_SELECT_STATE[] =
{
		//moving through files
		{FileSelection_SelectFile, PLAYPAUSE_EV, FILE_SELECT_STATE},
		{FileSelection_NextFile, NEXT_EV, FILE_SELECT_STATE},
		{FileSelection_PreviousFile, PREV_EV, FILE_SELECT_STATE},

		//Encoder
		//{Effects_InitState, ENCODER_PRESS_EV, EFFECTS_STATE},
		{FileSelection_NextFile, ENCODER_RIGHT_EV, FILE_SELECT_STATE},
		{FileSelection_PreviousFile, ENCODER_LEFT_EV, FILE_SELECT_STATE},

		//Turn off/Reset
		{Idle_InitState, ENCODER_LKP_EV, IDLE_STATE},

		//SD
		{Idle_InitState, SD_OUT_EV, IDLE_STATE},
		{Idle_InitState, TIMEOUT_EV, IDLE_STATE},


		//File Selected
		//{Player_InitState, FILE_SELECTED_EV, AUDIO_PLAYER_STATE},

		//Audio Player
		//{Audio_updateAll, FILL_BUFFER_EV, FILE_SELECT_STATE},
		{FileSelection_PlayNextSong, NEXT_SONG_EV, FILE_SELECT_STATE},
		{FileSelection_PlayPrevSong, PREV_SONG_EV, FILE_SELECT_STATE},

		//End of Table
		{pass, END_TABLE, FILE_SELECT_STATE}
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
