/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>


#include "../HAL/memory_handler.h"
#include "mp3_handler/mp3_handler.h"
#include "power_mode_switch.h"
#include "AudioPlayer.h"
#include "../HAL/mp3_decoder.h"
#include "../HAL/matrix_display.h"
#include "fft/vumeter.h"
#include "equalizer/equalizer.h"
#include "EventQueue/queue.h"

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/



/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/



/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/* Función que se llama 1 vez, al comienzo del programa */
void App_Init_test (void)
{
	PowerMode_Init();
	queue_Init();
	memory_handler_init();
	MP3Decoder_Init();
	md_Init();
	AudioPlayer_Init();
	VU_Init();
	EQ_Init();
}


/* Función que se llama constantemente en un ciclo infinito */
void App_Run_test (void)
{
	if(mh_SD_connected())
	{
		// Fetch the sd on conection event
		mp3Handler_init();
		mp3Handler_selectObject();

		// Selecciono canción
		mp3Handler_selectObject();

		mp3Handler_play();

		while(1)
		{
			if(AudioPlayer_IsBackBufferFree())
			{
				 mp3Handler_updateAll();
			}
			if(get_Queue_Status())
			{
				Event_Type event;
				event = pull_Queue_Element();

				// TODO: hacer que pase a la siguiente canción
				if (event == NEXT_SONG_EV)
				{
					mp3Handler_playPreviousSong();
				}
			}
		}

	}

}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/






