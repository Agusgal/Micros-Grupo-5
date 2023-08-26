/***************************************************************************/ /**
  @file     add_user_id_state.c
  @brief    ID state functions
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdbool.h>
#include "Timer.h"
#include "add_user_id_state.h"
#include "user_input.h"
#include "lector.h"
#include "seven_seg_display.h"
#include "data_base.h"
#include "queue.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define TITLE_TIME  8000

/*******************************************************************************
 * GLOBAL VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static int8_t id[ID_ARRAY_SIZE];
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
 * @brief Function executed when the ID is not correct.
 */
static void idFail(void);

/**
 * @brief Id already used in database
 */
static void idAlreadyExists(void);
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
//!OJO EN TODAS ESTA HABRIA QUE RESETEAR EL TIMER DE TIMEOUT Y EN ALGUNAS ACTUALIZAR EL DISPLAY
//TODO AGREGAR ESO

void addUserId_initLogin(void)
{
    showingErrorIndication = false;
    inputResetArray(id, &currentPos, ID_ARRAY_SIZE);
    showTitle();
}

void addUserId_increaseCurrent(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if (showingErrorIndication)
    	userInteractionStopsErrorIndication();
    else
        inputIncreaseCurrent(id, currentPos);
}

void addUserId_decreaseCurrent(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if (showingErrorIndication)
    	userInteractionStopsErrorIndication();
    else
        inputDecreaseCurrent(id, currentPos);
}

void addUserId_confirmID(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if (showingErrorIndication)
    	userInteractionStopsErrorIndication();
    else
    {
        Status idValidation = validateNewId(id);
        if (idValidation == VALIDATE_SUCCESSFULL)
            emitEvent(ID_OK_EV);
        else if (idValidation == ID_ALREADY_EXISTS)
            idAlreadyExists();
        else if (idValidation == ID_WRONG_FORMAT)
            idFail();
    }
}


void addUserId_acceptNumber(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if (showingErrorIndication)
    	userInteractionStopsErrorIndication();
    else
        inputAcceptNumber(id, &currentPos, ID_ARRAY_SIZE);
}

int8_t * addUserId_getIDArray(void)
{
    return id;
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
    SevenSegDisplay_WriteBuffer("ID  ", 4, 0);
    showingTitle = true;
    titleTimerID = Timer_AddCallback(&stopShowingTitle, TITLE_TIME, true);
}

static void stopShowingTitle(void)
{
    SevenSegDisplay_EraseScreen();
    SevenSegDisplay_SetPos(0);
    SevenSegDisplay_WriteBuffer((char *)id, ID_ARRAY_SIZE, 0);
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
    addUserId_initLogin();
}

static void idFail(void)
{
    SevenSegDisplay_EraseScreen();
    SevenSegDisplay_CursorOff();
    SevenSegDisplay_SetPos(0);
    SevenSegDisplay_WriteBufferAndMove("ID FAILED", 9, 0, SHIFT_L);
    showingErrorIndication = true;
    errorIndicationTimerID = Timer_AddCallback(&addUserId_initLogin, TITLE_TIME, true);
}


static void idAlreadyExists(void)
{
    SevenSegDisplay_EraseScreen();
    SevenSegDisplay_CursorOff();
    SevenSegDisplay_SetPos(0);
    SevenSegDisplay_WriteBufferAndMove("ID IS USED", 10, 0, SHIFT_L);
    showingErrorIndication = true;
    errorIndicationTimerID = Timer_AddCallback(&addUserId_initLogin, TITLE_TIME, true);
}
