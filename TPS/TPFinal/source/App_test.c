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
	memory_handler_init();
	MP3Decoder_Init();
	AudioPlayer_Init();
}


/* Función que se llama constantemente en un ciclo infinito */
void App_Run_test (void)
{
	if(mh_SD_connected())
	{
		// Fetch the sd on conection event
		mp3Handler_init();
		mp3Handler_nextObject();
		mp3Handler_nextObject();
		mp3Handler_selectObject();

	}

}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/






