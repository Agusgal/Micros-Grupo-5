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
#include "States/equalizer_state.h"
#include "States/audioPlayer_state.h"


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
		{Effects_InitState, ENCODER_PRESS_EV, EQUALIZER_STATE},
		{FileSelection_NextFile, ENCODER_RIGHT_EV, FILE_SELECT_STATE},
		{FileSelection_PreviousFile, ENCODER_LEFT_EV, FILE_SELECT_STATE},

		//Turn off/Reset
		{Idle_InitState, ENCODER_LKP_EV, IDLE_STATE},

		//SD
		{Idle_InitState, SD_OUT_EV, IDLE_STATE},
		{Idle_InitState, TIMEOUT_EV, IDLE_STATE},

		//File Selected
		{Player_InitState, FILE_SELECTED_EV, AUDIO_PLAYER_STATE},

		//Audio Player
		{FileSelection_MP3_UpdateAll, FILL_BUFFER_EV, FILE_SELECT_STATE},
		{FileSelection_PlayNextSong, NEXT_SONG_EV, FILE_SELECT_STATE},
		{FileSelection_PlayPrevSong, PREV_SONG_EV, FILE_SELECT_STATE},

		//End of Table
		{pass, END_TABLE, FILE_SELECT_STATE}
};


state AUDIO_PLAYER_STATE[] =
{
		//Buttons
		{Player_ToggleMusic, PLAYPAUSE_EV, AUDIO_PLAYER_STATE},
		{Player_Stop, STOP_EV, AUDIO_PLAYER_STATE},
		{Player_PlayNextSong, NEXT_EV, AUDIO_PLAYER_STATE},
		{Player_PlayPreviousSong, PREV_EV, AUDIO_PLAYER_STATE},

		//Encoder
		{FileSelection_InitState, ENCODER_PRESS_EV, FILE_SELECT_STATE},
		{Player_IncVolume, ENCODER_RIGHT_EV, AUDIO_PLAYER_STATE},
		{Player_DecVolume, ENCODER_LEFT_EV, AUDIO_PLAYER_STATE},

		//Con long key press pongo efectos, todo: chequear funcionamiento, pausas, etc
		{Effects_InitState, ENCODER_LKP_EV, EQUALIZER_STATE},

		//SD
		{Idle_InitState, SD_OUT_EV, IDLE_STATE},
		{Idle_InitState, TIMEOUT_EV, IDLE_STATE},

		//Audio
		{Player_MP3_UpdateAll, FILL_BUFFER_EV, AUDIO_PLAYER_STATE},
		{Player_PlayNextSong, NEXT_SONG_EV, AUDIO_PLAYER_STATE},
		{Player_PlayPreviousSong, PREV_SONG_EV, AUDIO_PLAYER_STATE},

		//End of Table
		{pass, END_TABLE, AUDIO_PLAYER_STATE}
};


state EQUALIZER_STATE[] =
{
		//Buttons
		{Effects_SelectOption, PLAYPAUSE_EV, EQUALIZER_STATE},
		//{Effects_Back, STOP_EV, EQUALIZER_STATE},
		{Effects_NextOption, NEXT_EV, EQUALIZER_STATE},
		{Effects_PreviousOption, PREV_EV, EQUALIZER_STATE},

		//Encoder, if pressed go back to file selection
		{FileSelection_InitState, ENCODER_PRESS_EV, FILE_SELECT_STATE},
		{Effects_NextOption, ENCODER_RIGHT_EV, EQUALIZER_STATE},
		{Effects_PreviousOption, ENCODER_LEFT_EV, EQUALIZER_STATE},

		//Apagar long key press
		{Idle_InitState, ENCODER_LKP_EV, IDLE_STATE},

		//SD
		{Idle_InitState, TIMEOUT_EV, IDLE_STATE},
		{Idle_InitState, SD_OUT_EV, IDLE_STATE},

		//Change mode if selected
		{FileSelection_InitState, CHANGE_MODE_EV, FILE_SELECT_STATE},

		//Audio
		{Equalizer_MP3_UpdateAll, FILL_BUFFER_EV, EQUALIZER_STATE},
		{FileSelection_PlayNextSong, NEXT_SONG_EV, EQUALIZER_STATE},
		{FileSelection_PlayPrevSong, PREV_SONG_EV, EQUALIZER_STATE},

		{pass, END_TABLE, EQUALIZER_STATE}
};


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/




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



state* get_initial_state()
{
	return INIT_STATE;
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
