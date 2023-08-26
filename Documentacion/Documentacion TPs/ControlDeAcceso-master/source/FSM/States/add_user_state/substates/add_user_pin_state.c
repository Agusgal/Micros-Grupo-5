/***************************************************************************/ /**
  @file     add_user_pin_state.c
  @brief    Add user Pin state functions
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdbool.h>
#include "timer.h"
#include "add_user_pin_state.h"
#include "user_input.h"
#include "data_base.h"
#include "queue.h"
#include "seven_seg_display.h"

#include "add_user_id_state.h"
#include "add_user_main_state.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define TITLE_TIME  6000
/*******************************************************************************
 * GLOBAL VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static int8_t pin[PIN_ARRAY_SIZE];
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
 */
static void userInteractionStopsTitle(void);

/**
 * @brief Stops showing the error indication in the display due to a user's interaction. The state 'restarts'.
 */
static void userInteractionStopsErrorIndication(void);

/**
 * @brief Function executed when the PIN is not correct.
 */
static void pinFail(void);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
//!OJO EN TODAS ESTADS HABRIA QUE RESETEAR EL TIMER DE TIMEOUT Y EN ALGUNAS ACTUALIZAR EL DISPLAY
//TODO AGREGAR ESO

void addUserPin_initPinInput(void)
{
    showingErrorIndication = false;
	inputResetArray(pin, &currentPos, PIN_ARRAY_SIZE);
    showTitle();
}

void addUserPin_confirmPin(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if (showingErrorIndication)
    	userInteractionStopsErrorIndication();
    else
    {
        if (checkPinArrayFormat(pin))
        {
            Status ok = checkAddUser(addUserId_getIDArray(), pin, addUsr_getCardNumber(), 19, USER);
            if(ok == STORE_SUCCESSFULL)
            {
                emitEvent(ADD_USER_FINISHED_EV);
                return;
            }
        }   
        pinFail();
    }    
}

void addUserPin_acceptNumber(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if (showingErrorIndication)
    	userInteractionStopsErrorIndication();
    else
    {
        char aux[] = {MID_LINE};
        inputAcceptNumber(pin, &currentPos, PIN_ARRAY_SIZE);
        SevenSegDisplay_WriteBuffer(aux, 1, currentPos-1);
    }
        
}

void addUserPin_increaseCurrent(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if (showingErrorIndication)
    	userInteractionStopsErrorIndication();
    else
        inputIncreaseCurrent(pin, currentPos);
}

void addUserPin_decreaseCurrent(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if (showingErrorIndication)
    	userInteractionStopsErrorIndication();
    else
        inputDecreaseCurrent(pin, currentPos);
}

int8_t *addUserPin_getPinArray(int *sizeOfReturningArray)
{
    int currentArrayLength = getEffectiveArrayLength(pin, PIN_ARRAY_SIZE);
    *sizeOfReturningArray = currentArrayLength;
    return pin;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void showTitle(void)
{
    SevenSegDisplay_EraseScreen();
    SevenSegDisplay_SetPos(0);
    SevenSegDisplay_CursorOff();
    SevenSegDisplay_WriteBuffer("PIN ", 4, 0);
    showingTitle = true;
    titleTimerID = Timer_AddCallback(&stopShowingTitle, TITLE_TIME, true);
}

static void stopShowingTitle(void)
{
    SevenSegDisplay_EraseScreen();
    SevenSegDisplay_SetPos(0);
    SevenSegDisplay_WriteBuffer((char *)pin, PIN_ARRAY_SIZE, 0);
    SevenSegDisplay_CursorOn();
    showingTitle = false;
}

static void userInteractionStopsTitle(void)
{
    Timer_Delete(titleTimerID);
    titleTimerID = -1;
    stopShowingTitle();
    SevenSegDisplay_CursorOn();
}

static void userInteractionStopsErrorIndication(void)
{
    SevenSegDisplay_EraseScreen();
    Timer_Delete(errorIndicationTimerID);
    showingErrorIndication = false;
    errorIndicationTimerID = -1;
    addUserPin_initPinInput();
}

static void pinFail(void)
{
    SevenSegDisplay_EraseScreen();
    SevenSegDisplay_CursorOff();
    SevenSegDisplay_SetPos(0);
    SevenSegDisplay_WriteBufferAndMove("PIN FAILED", 9, 0, SHIFT_L);
    showingErrorIndication = true;
    errorIndicationTimerID = Timer_AddCallback(&addUserPin_initPinInput, TITLE_TIME, true);
}
