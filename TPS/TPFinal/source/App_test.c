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
#include "mp3_file_handler/mp3_file_handler.h"

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

}



/* Función que se llama constantemente en un ciclo infinito */
void App_Run_test (void)
{
	MP3Object_t prueba;
	MP3Object_t prueba2;
	if(mh_SD_connected())
	{
		// Fetch the sd on conection event
		mp3Handler_init();
		prueba  = mp3Files_GetFirstObject();
		prueba2 = mp3Files_GetNextObject(prueba);
		prueba2 =  mp3Files_Enter_Dir(prueba2);
		prueba2 = mp3Files_GetNextObject(prueba2);
		prueba2 =  mp3Files_Enter_Dir(prueba2);

		prueba2 = mp3Files_GetNextObject(prueba2);
		prueba2 =  mp3Files_Enter_Dir(prueba2);

		prueba2 = mp3Files_GetNextObject(prueba2);
		prueba2 =  mp3Files_Enter_Dir(prueba2);

		prueba = mp3Files_Exit_Dir(prueba2);
		prueba = mp3Files_Exit_Dir(prueba);

		prueba = mp3Files_Exit_Dir(prueba);
		prueba = mp3Files_Exit_Dir(prueba);
		prueba = mp3Files_Exit_Dir(prueba);
	}

}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/






