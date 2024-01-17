/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <FSM_1/FSM.h>
#include "MK64F12.h"
#include "hardware.h"

#include "Drivers/HAL/Encoder.h"
#include "board.h"
#include "Drivers/MCAL/gpio.h"

#include "AudioPlayer.h"

#include "EventQueue/queue.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


//TODO: que hace esto aca?????
void SysTick_Init(void);

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
	//SysTick_Init();
	//Timer_Init();

	//Memory Init
	//OLED Init
	//Matrix Init

	//MP3 decoder Init

	//Audio PLayer Init
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

	//TODO: cambiar
    hw_DisableInterrupts();
    SysTick_Init();
    hw_EnableInterrupts();
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

	//check for encoder events


	//Check for Encoder press events


	//Check for button events


	//Check for OLEd events


	//Check for AudioPLayer Events
	if (AudioPlayer_IsBackBufferFree())
	{
		push_Queue_Element(FILL_BUFFER_EV);
	}


	//Check for memory events
}



/*
 *---------------------PROGRAMA DE PRUEBA PANTALLA SSD1306 - 128*64 ---------------------
 *
 * */

/*

#include <stdio.h>
#include "board.h"
//#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_i2c.h"
//#include "fsl_swm.h"
#include "OLEDdisplay.h"


//   Variables
uint32_t baudRate = 400000;
uint32_t frecuency = 12000000;

int testFunc(void)
{
	i2c_master_config_t  i2config;                                    // config. variable. i2c

	//CLOCK_Select(kI2C1_Clk_From_MainClk);

	CLOCK_EnableClock(kCLOCK_Swm);
	SWM_SetMovablePinSelect(SWM0, kSWM_I2C1_SDA, kSWM_PortPin_P0_28);  // SDA to PIO0_28
	SWM_SetMovablePinSelect(SWM0, kSWM_I2C1_SCL, kSWM_PortPin_P0_27);  // SCL to PIO0_27
	CLOCK_DisableClock(kCLOCK_Swm);

	I2C_MasterGetDefaultConfig(&i2config);                             // get the default config of i2c

//------DEFAULT CONFIG---------------
//	  masterConfig.enableMaster  = true;
//    masterConfig.baudRate_Bps  = 100000U;
//    masterConfig.enableTimeout = false;


	i2config.baudRate_Bps = baudRate;                                  //adjusting the baudrate to the OLED oscillator

	I2C_MasterInit(I2C1, &i2config, frecuency);                        //initialization

    // Initialize the SSD1306 display
    OLED_Init();
    OLED_Refresh();
    OLED_Clear();

    // Print welcome message
    OLED_Copy_Image(&logo_nxp[0], sizeof(logo_nxp));
    OLED_Refresh();

    OLED_Set_Text(10, 52, kOLED_Pixel_Set, "HELLO WORLD", 2);
    OLED_Refresh();


    while(1) {

    }
    return 0 ;
}

*/
