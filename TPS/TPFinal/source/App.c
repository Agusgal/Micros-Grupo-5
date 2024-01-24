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
#include "Buttons.h"
#include "Encoder.h"

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
	Buttons_Init();

	//Init fsm
	current_state = get_initial_state();
	start_fsm();

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

	Event_Type button_event;
	for (int button = 0; button < BUTTON_SIZE; button++)
	{
		button_event = getButtonEvent(button);

		if(button_event != NO_BUTTON_PRESS_EV)
		{
			push_Queue_Element(button_event);
		}

		if (button_event == PLAYPAUSE_EV)
		{
			static int count = 0;
			count++;
		}
	}


	//check for encoder turn events
	int move_enc = getEncoder_State();
	if (move_enc == 1) //move right
	{
		push_Queue_Element(ENCODER_RIGHT_EV);
	}
	else if (move_enc == 2)
	{
		push_Queue_Element(ENCODER_LEFT_EV);
	}

	//Check for Encoder press events
	int encoder_state = getEncoderSwitch_State();
	if (encoder_state == RELEASED)
	{
		push_Queue_Element(ENCODER_PRESS_EV);
	}
	else if(encoder_state == FIVE_SEC_PRESS)
	{
		push_Queue_Element(ENCODER_LKP_EV);
	}

	//Check for OLED events
}




