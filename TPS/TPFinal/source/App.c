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


#include <FSM_1/FSM.h>

#include "Drivers/HAL/Encoder.h"

#include "fsl_common.h"
#include "power_mode_switch.h"

#include "gpio.h"
#include "board.h"

#include "SysTick.h"
#include "Timer.h"

#include "OLEDdisplay.h"
#include "matrix_display.h"

#include "AudioPlayer.h"

#include "EventQueue/queue.h"

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void testFunc(void);


/**
 * @brief captures events generated within the drivers and inside states and fills the eventqueue. Gets called continously.
 * @return nothing.
 */
void fill_queue(void);

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


//this variable saves the current state of the FSM
static state *current_state;


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	PowerMode_Init();

	SysTick_Init();
	Timer_Init();

	//Memory Init


	// Initialize the SSD1306 OLED display
	OLED_Init();
	OLED_Refresh();
	OLED_Clear();

	//Matrix Init
	md_Init();  //Agrego implementacion de la documentacion para poder inicializar el audio player.

	//MP3 decoder Init

	//Audio PLayer Init, para poder inicializarlo debe estar inicializado el DMA, sino tira error turbio.
	AudioPlayer_Init();

	//Vumeter/FFT Init
	//Equalizer Init

	//Daytime Init

	//Init Queue
	queue_Init();

	//Init Encoder
	Encoder_Init();

	//Init fsm
	current_state = get_initial_state();
	start_fsm();

	//testFunc();
}



/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	fill_queue();

	Event_Type event = pull_Queue_Element();

	if (event != NONE_EV)
	{
		current_state = fsm_dispatcher(current_state, event);
	}

}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/**
 * @brief captures events generated within the drivers and inside states and fills the eventqueue. Gets called continously.
 * @return nothing.
 */
void fill_queue(void)
{
	//If OLED Init worked... begin. (only emits event one time)
	static bool OLEDdone = false;
	if(!OLEDdone && OLEDisInit())
	{
		OLEDdone = true;
		push_Queue_Element(START_EV);
	}

	//Check for memory events
	/*
	if(Mm_SDConnection())
	{
		push_Queue_Element(SD_IN_EV);
	}

	if(Mm_SDDesconnection())
	{
		push_Queue_Element(SD_OUT_EV);
	}
	*/

	//Check for AudioPLayer Events
	if (AudioPlayer_IsBackBufferFree())
	{
		push_Queue_Element(FILL_BUFFER_EV);
	}

	//check for encoder events


	//Check for Encoder press events


	//Check for button events


	//Check for OLED events
}



/*
 *---------------------PROGRAMA DE PRUEBA PANTALLA SSD1306 - 128*64 ---------------------
 *
 * */

void testFunc(void)
{
    // Print welcome message
    //OLED_Copy_Image(&logo_nxp[0], sizeof(logo_nxp));
    //OLED_Refresh();
    //OLED_Clear();

    //OLED_Set_Text(0, 52, kOLED_Pixel_Set, "ABCDEFGHIJKLMNOPQ", 2);
    //OLED_Refresh();
}




