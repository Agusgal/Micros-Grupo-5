/***************************************************************************/ /**
  @file     file_selection_state.c
  @brief    File Selection State Functions
  @author   Grupo 5 - Lab de Micros
 ******************************************************************************/

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdbool.h>
#include <string.h>

#include "../../mp3_handler/mp3_handler.h"

#include "file_selection.h"
#include "../../EventQueue/queue.h"


#include "Timer.h"
#include "OLEDdisplay.h"

//#include "memory_manager.h"
//#include "file_system_manager.h"
//#include "decoder.h"

#include "power_mode_switch.h"

/*******************************************************************************
 * GLOBAL VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/



/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


/**
 * @brief Show the title of the state on the display. If the user interacts with the system, the title will stop showing.
 */
static void showTitle(void);



/**
 * @brief Prints the file's name and artist
 */
static void printFileInfo(void);


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void FileSelection_InitState(void)
{
	//todo: mejorar OLED para mostrar mejor la pantalla de titulo...
	showTitle();

	mp3Handler_init();

	printFileInfo();
}

void FileSelection_NextFile(void)
{
	mp3Handler_nextObject();
	printFileInfo();
}

void FileSelection_PreviousFile(void)
{
	mp3Handler_prevObject();
	printFileInfo();
}

void FileSelection_SelectFile(void)
{
	// AGUS PRESTÁ ATENCIÓN, NO DEBERÍA ROMPER NADA, PERO FIJATE
	// SI NO ES UN MP3 FILE, LA FSM DEBERÍA SEGUIR EN EL MISMO ESTADO, COMO SI NO HUBIERA
	// PASADO NADA

	// If the file is an MP3 File
	if(mp3Handler_selectObject())
	{
		/* Start decoding the file and play the audio player */
		mp3Handler_play();
		push_Queue_Element(FILE_SELECTED_EV);
		return;
	}
}


void FileSelection_PlayNextSong(void)
{
	mp3Handler_playNextSong();
}


void FileSelection_PlayPrevSong(void)
{
	mp3Handler_playPreviousSong();
}


void FileSelection_MP3_UpdateAll(void)
{
	mp3Handler_updateAll();
}

/*******************************************************************************
 *******************************************************************************
 *                         LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void showTitle(void)
{
	OLED_Clear();
	OLED_Refresh();
	OLED_write_Text(10, 16, "Choose file:");
}


static void printFileInfo(void)
{
	//todo: ojo con filenames muy grandes, conviene agrandar buffer auxiliar.

	//OLED_Clear();
	//Get file name from audio module
	char * name = mp3Handler_getCurrentName();
	uint8_t path[150];


	memset(path, 0x20, 50);
	memcpy(path, name, strlen(name));

	//todo: hay que ver si el casteo éste está bien, parece que no xd
	OLED_write_Text(10, 32, (char*)path);
}
