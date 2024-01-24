/***************************************************************************/ /**
  @file     file_selection_state.c
  @brief    File Selection State Functions
  @author   Grupo 5 - Lab de Micros
 ******************************************************************************/

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

//todo: tambien vuela.
#define TITLE_TIME 5000

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdbool.h>
#include <string.h>

#include "../../mp3_handler/mp3_handler.h"
//#include "AudioPlayer.h"

#include "file_selection.h"
#include "../../EventQueue/queue.h"


#include "Timer.h"
#include "OLEDdisplay.h"
//#include "LCD_GDM1602A.h"
//#include "memory_manager.h"
//#include "file_system_manager.h"
//#include "decoder.h"

#include "power_mode_switch.h"

/*******************************************************************************
 * GLOBAL VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static bool showingTitle;
static int titleTimerID = -1;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


/**
 * @brief Show the title of the state on the display. If the user interacts with the system, the title will stop showing.
 */
static void showTitle(void);


/**
 * @brief Stops showing the title of the state on the display.
 */
static void stopShowingTitle(void);


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
	showTitle();
	//Audio_init();

	printFileInfo();
}

void FileSelection_NextFile(void)
{
	//Audio_nextFile();
	//printFileInfo();
}

void FileSelection_PreviousFile(void)
{
	//Audio_prevFile();
	//printFileInfo();
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
		//Audio_play();
		push_Queue_Element(FILE_SELECTED_EV);
		return;
	}
}

void FileSelection_PlayNextSong(void)
{
	//Audio_playNextFile();
	//Audio_play();
}

void FileSelection_PlayPrevSong(void)
{
	//Audio_playPrevFile();
	//Audio_play();
}


/*******************************************************************************
 *******************************************************************************
 *                         LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void showTitle(void)
{
	OLED_Clear();
	OLED_write_Text(20, 22, "Choose file:");

	//showingTitle = true;
	//titleTimerID = Timer_AddCallback(&stopShowingTitle, TITLE_TIME, true);
}

//todo esto vuela me parece
static void stopShowingTitle(void)
{
	showingTitle = false;
	//LCD_clearDisplay();
	printFileInfo();
	//showFiles();
}



static void printFileInfo(void)
{
	//todo: ojo con filenames muy grandes, conviene agrandar buffer auxiliar.
	//LCD_clearDisplay();

	//Get file name from audio module
	//char * name = Audio_getCurrentName();
	uint8_t path[50];


	//memset(path, 0x20, 50);
	//memcpy(path, name, strlen(name));

	//OLED_write_Text(20, 42, path);

}
