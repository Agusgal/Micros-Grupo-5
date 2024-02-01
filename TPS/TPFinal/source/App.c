/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Grupo 5
 ******************************************************************************/

/*
 * todo:	-Hacer andar el OLED con el scroll
 *
 *			- Agregar que se pueda cambiar de efectos desde reproduccion de sonido
 *
 * 			- Testear estado de efectos, en especial banda customizada
 *			- Testear/debuguear matriz OLED
 *
 *
 *			- Poder salir de la carpeta, al estar reproduciendo
 *			(dar opción de navegar mientras se reproduce una canción)
 *
 *			- Hacer que al pasar de canción, o terminar la canción, el buffer quede con el último
 *			valor escrito (para evitar el sonido entre canciones)
 *
 *			- Previous song, si se presiona 2 veces en 3 segundos, vaya a la canción anterior
 * 				En caso contrario, vuelva a reproducir la canción actual
 *
 *
 */

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>


#include <FSM_1/FSM.h>

#include "mp3_decoder.h"

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
#include "vumeter.h"

#include "AudioPlayer.h"

#include "EventQueue/queue.h"

#include "memory_handler.h"

#include "equalizer.h"

#include "mp3_handler/mp3_handler.h"


#include "board.h"
/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


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

	//Init Queue
	queue_Init();

	//Memory Init
	memory_handler_init();


	//MP3 decoder Init
	MP3Decoder_Init();

	//Matrix Init
	md_Init();

	//Audio PLayer Init, para poder inicializarlo debe estar inicializado el DMA, sino tira error turbio.
	AudioPlayer_Init();

	//Vumeter Init
	VU_Init();

	//Equalizer Init
	EQ_Init();

	// Initialize the SSD1306 OLED display
	OLED_Init();
	OLED_Refresh();
	//OLED_Clear();


	//todo: Daytime Init


	//Init Encoder
	Encoder_Init();

	//Init Buttons
	Buttons_Init();

	gpioMode(TP,OUTPUT);
	gpioMode(TP2,OUTPUT);


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
	if(mh_SD_connected())
	{
		push_Queue_Element(SD_IN_EV);
	}

	if(mh_SD_disconnected())
	{
		push_Queue_Element(SD_OUT_EV);
	}


	//Check for AudioPLayer Events
	if (AudioPlayer_IsBackBufferFree())
	{
		//push_Queue_Element(FILL_BUFFER_EV);
		mp3Handler_updateAll();
	}


	//Check for Button Events
	Event_Type button_event;
	for (int button = 0; button < BUTTON_SIZE; button++)
	{
		button_event = getButtonEvent(button);

		if(button_event != NO_BUTTON_PRESS_EV)
		{
			push_Queue_Element(button_event);
		}
	}


	//check for Encoder turn events
	int move_enc = getEncoder_State();
	if (move_enc == 1)
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
}
