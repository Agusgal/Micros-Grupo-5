/***************************************************************************/ /**
  @file     id_state.c
  @brief    ID state functions
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdbool.h>
#include "Timer.h"
#include "id_state.h"
#include "user_input.h"
#include "lector.h"
#include "seven_seg_display.h"
#include "data_base.h"
#include "queue.h"
#include "./drivers/Led.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define TITLE_TIME  6000

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
static void userInteractionStopErrorIndicationAndRestart(void);

/**
 * @brief Stops showing the error indication in the display due to a user's interaction. The state doesn't 'restart'.
 */
static void userInteractionStopsErrorIndication(void);

/**
 * @brief Function executed when the ID is not correct.
 */
static void idFail(void);
/**
 * @brief Function executed when the lector fails to read the card.
 */
static void idCardFail(void);

static void id_fail_animate(void);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
//!OJO EN TODAS ESTA HABRIA QUE RESETEAR EL TIMER DE TIMEOUT Y EN ALGUNAS ACTUALIZAR EL DISPLAY
//TODO AGREGAR ESO

void initLogin(void)
{
    Led_On(LED_BLUE);
	SevenSegDisplay_StopAnimation();
    showingErrorIndication = false;
    inputResetArray(id, &currentPos, ID_ARRAY_SIZE);
    showTitle();
}

void id_increaseCurrent(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if (showingErrorIndication)
    {
        userInteractionStopErrorIndicationAndRestart();  
    	SevenSegDisplay_StopAnimation();
    }
    else
        inputIncreaseCurrent(id, currentPos);
}

void id_decreaseCurrent(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if (showingErrorIndication)
    {
        userInteractionStopErrorIndicationAndRestart();
    	SevenSegDisplay_StopAnimation();
    }
    else
        inputDecreaseCurrent(id, currentPos);
}

void id_confirmID(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if (showingErrorIndication)
    {
        userInteractionStopErrorIndicationAndRestart();
    	SevenSegDisplay_StopAnimation();
    }
    else
    {
        if (!verifyID(id))
            idFail();
        else
            emitEvent(ID_OK_EV);
    }
}


void id_acceptNumber(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if (showingErrorIndication)
    {
        userInteractionStopErrorIndicationAndRestart();
    	SevenSegDisplay_StopAnimation();
    }
    else
        inputAcceptNumber(id, &currentPos, ID_ARRAY_SIZE);
}

int8_t * id_getIdArray(int *sizeOfReturningArray)
{
    int currentArrayLength = getEffectiveArrayLength(id, ID_ARRAY_SIZE);
    *sizeOfReturningArray = currentArrayLength;
    return id;
}

void id_checkCardID(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if (showingErrorIndication)
        userInteractionStopsErrorIndication();
    card_t myCard;
    bool ok = Lector_GetData(&myCard);
    if(ok)
    {
        // agarro numero de tarjeta
        int8_t numero[19];
        uint8_t i,length = myCard.number_len;
        for(i = 0; i<length; i++)
        {
            numero[i] = myCard.card_number[i];
        }
        
        if(verifyCardNumber(numero, length))
        {
            emitEvent(ID_OK_EV);
        }
        else
        {
            idFail();
        }
    }
    else
    {
        idCardFail();
    }
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

static void userInteractionStopErrorIndicationAndRestart(void)
{
    userInteractionStopsErrorIndication();
    initLogin();
    Led_StopInfiniteBlink(LED_RED);
    Led_On(LED_BLUE);
}

static void userInteractionStopsErrorIndication(void)
{
    SevenSegDisplay_EraseScreen();
    Timer_Delete(errorIndicationTimerID);
    showingErrorIndication = false;
    errorIndicationTimerID = -1;
    Led_StopInfiniteBlink(LED_RED);
    Led_On(LED_BLUE);
}

static void idFail(void)
{
    SevenSegDisplay_EraseScreen();
    SevenSegDisplay_CursorOff();
    SevenSegDisplay_SetPos(0);
    if(isCurrentUserBlocked())
    {
    	SevenSegDisplay_WriteBufferAndMove("USER BLOCKED", 12, 0, SHIFT_L);
    }
    else
    {
    	SevenSegDisplay_WriteBufferAndMove("NO ID FOUND", 11, 0, SHIFT_L);
    }
    showingErrorIndication = true;
    errorIndicationTimerID = Timer_AddCallback(&id_fail_animate, TITLE_TIME, true);
    Led_Off(LED_BLUE);
    Led_InfiniteBlink(LED_RED, NORMAL);
}

static void id_fail_animate(void)
{
	showingErrorIndication = false;
	SevenSegDisplay_EraseScreen();
	SevenSegDisplay_SetPos(0);
	SevenSegDisplay_AnimationCircles();
	showingErrorIndication = true;
	errorIndicationTimerID = Timer_AddCallback(&initLogin, 600, true);
	Led_StopInfiniteBlink(LED_RED);
    Led_On(LED_BLUE);
}

static void idCardFail(void)
{
    SevenSegDisplay_EraseScreen();
    SevenSegDisplay_CursorOff();
    SevenSegDisplay_SetPos(0);
    SevenSegDisplay_WriteBufferAndMove("CARD FAILED", 11, 0, SHIFT_L);
    showingErrorIndication = true;
    //errorIndicationTimerID = Timer_AddCallback(&initLogin, TITLE_TIME, true);
    errorIndicationTimerID = Timer_AddCallback(&id_fail_animate, TITLE_TIME, true);
    Led_Off(LED_BLUE);
    Led_InfiniteBlink(LED_RED, NORMAL);
}
