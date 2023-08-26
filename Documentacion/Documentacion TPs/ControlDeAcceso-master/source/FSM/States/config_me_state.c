/***************************************************************************/ /**
  @file     config_me_state.c
  @brief    Config me state functions.
            The main function of the state is to change the user's PIN.
            The cycle of the process is: 
                1. Shows title
                2. User inserts new PIN.
                3a. PIN ok -> Returns to the menu.
                3b. PIN fail -> Shows error message and starts the cycle again.
            Any interaction of the user with the device during an error message or title will be interprated as an indication to stop the message/title.
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define TITLE_TIME  5000

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdbool.h>
#include "timer.h"
#include "config_me_state.h"
#include "user_input.h"
#include "data_base.h"
#include "queue.h"
#include "seven_seg_display.h"


/*******************************************************************************
 * GLOBAL VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static int8_t newPin[PIN_ARRAY_SIZE];
static uint8_t currentPos = 0;
static bool showingTitle, showingErrorIndication;
static int titleTimerID = -1;
static int errorIndicationTimerID = -1;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
/**
 * @brief Show the title of the state in the display. If the user interacts with the system, the title will stop showing and the input will start.
 */
static void showTitle(void);
/**
 * @brief Stops showing the title of the state in the display. The input starts.
 */
static void stopShowingTitle(void);

/**
 * @brief Stops showing the title of the state in the display due to a user's interaction. The input starts.
 * The main reason of creating another function for this is to avoid cancelling a callback (of the timer) inside of it (callback).
 */
static void userInteractionStopsTitle(void);

/**
 * @brief Stops showing the error indication in the display due to a user's interaction. The state 'restarts'.
 */
static void userInteractionStopsErrorIndication(void);

/**
 * @brief Function executed when the PIN format is not correct.
 */
static void pinFail(void);
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
//!OJO EN TODAS ESTADS HABRIA QUE RESETEAR EL TIMER DE TIMEOUT Y EN ALGUNAS ACTUALIZAR EL DISPLAY
//TODO AGREGAR ESO

void configMe_initState(void)
{
    showingErrorIndication = false;
	inputResetArray(newPin, &currentPos, PIN_ARRAY_SIZE);   //Initializes the input array.
    showTitle();    //Shows the state's title.
}

void configMe_confirmPin(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if (showingErrorIndication)
        userInteractionStopsErrorIndication(); 
    else
    {
        if (changePin(newPin) == PIN_CHANGED_SUCCESFULLY)   //The new pin format is correct.
            emitEvent(RETURN_TO_LAST_STATE_EV); //Returns to the menu.
        else
            pinFail();  //Invalid pin format.
    }
    
}

void configMe_acceptNumber(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if (showingErrorIndication)
        userInteractionStopsErrorIndication();  
    else
        inputAcceptNumber(newPin, &currentPos, PIN_ARRAY_SIZE);
}

void configMe_increaseCurrent(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if (showingErrorIndication)
        userInteractionStopsErrorIndication();  
    else
        inputIncreaseCurrent(newPin, currentPos);
}

void configMe_decreaseCurrent(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if (showingErrorIndication)
        userInteractionStopsErrorIndication();  
    else
        inputDecreaseCurrent(newPin, currentPos);
}
/*******************************************************************************
 *******************************************************************************
 *                         LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void showTitle(void)
{
    SevenSegDisplay_EraseScreen();
    SevenSegDisplay_SetPos(0);
    SevenSegDisplay_CursorOff();
    SevenSegDisplay_WriteBufferAndMove("INSERT PIN", 10, 0, BOUNCE);    //Shows title.
    showingTitle = true;
    titleTimerID = Timer_AddCallback(&stopShowingTitle, TITLE_TIME, true);  //Sets the callback to stop the title and start the input.
}

static void stopShowingTitle(void)
{
    SevenSegDisplay_EraseScreen();
    SevenSegDisplay_SetPos(0);
    SevenSegDisplay_WriteBuffer((char *)newPin, PIN_ARRAY_SIZE, 0); //Initializes the input.
    SevenSegDisplay_CursorOn(); 
    showingTitle = false;
}

static void userInteractionStopsTitle(void)
{
    Timer_Delete(titleTimerID); //Cancels the callback.
    titleTimerID = -1;
    stopShowingTitle(); 
}


static void userInteractionStopsErrorIndication(void)
{
    SevenSegDisplay_EraseScreen();
    Timer_Delete(errorIndicationTimerID);   //Cancels the callaback.
    showingErrorIndication = false;
    errorIndicationTimerID = -1;
    configMe_initState();   //Starts the state's cycle again.
}


static void pinFail(void)
{
    SevenSegDisplay_EraseScreen();
    SevenSegDisplay_CursorOff();
    SevenSegDisplay_SetPos(0);
    SevenSegDisplay_WriteBufferAndMove("PIN ERROR", 16, 0, BOUNCE); //Starts error indication message
    showingErrorIndication = true;
    errorIndicationTimerID = Timer_AddCallback(&configMe_initState, TITLE_TIME, true);  //Sets the callback to start the state's cycle again.
}
