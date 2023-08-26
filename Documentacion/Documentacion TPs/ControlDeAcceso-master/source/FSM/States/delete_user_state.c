/***************************************************************************/ /**
  @file     delete_user_state.c
  @brief    Delete user state functions
            The main function of the state is to delete an user.
            The cycle of the process is: 
                1. Shows title
                2. Admin inserts ID or the card of the user to delete.
                3a. ID ok -> Deletes the user and returns to the menu.
                3b. ID fail -> Shows error message and starts the cycle again.
            Any interaction of the user with the device during an error message or title will be interprated as an indication to stop the message/title.
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>
#include "Timer.h"
#include "delete_user_state.h"
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
 * The main reason of creating another function for this is to avoid cancelling a callback (of the timer) inside of it (callback).
 */
static void userInteractionStopsTitle(void);

/**
 * @brief Stops showing the error indication in the display due to a user's interaction. The state 'restarts'.
 */
static void userInteractionStopErrorIndicationAndRestart(void);

/**
 * @brief Stops showing the error indication in the display due to a user's interaction. The state doesn't 'restart'.
 * The main reason of creating another function for this is to avoid cancelling a callback (of the timer) inside of it (callback).
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

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void deleteUser_initState(void)
{
    showingErrorIndication = false;
    inputResetArray(id, &currentPos, ID_ARRAY_SIZE);
    showTitle();
}

void deleteUser_increaseCurrent(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if (showingErrorIndication)
        userInteractionStopErrorIndicationAndRestart();  
    else
        inputIncreaseCurrent(id, currentPos);
}

void deleteUser_decreaseCurrent(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if (showingErrorIndication)
        userInteractionStopErrorIndicationAndRestart();  
    else
        inputDecreaseCurrent(id, currentPos);
}

void deleteUser_confirm(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if (showingErrorIndication)
        userInteractionStopErrorIndicationAndRestart();    
    else
    {
        if (removeUser(id) == DELETE_SUCCESSFULL)
            emitEvent(ID_OK_EV);
        else
            idFail();
            
    }
}

void deleteUser_acceptNumber(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else if (showingErrorIndication)
        userInteractionStopErrorIndicationAndRestart();
    else
        inputAcceptNumber(id, &currentPos, ID_ARRAY_SIZE);
}

int8_t * deleteUser_getIdArray(int *sizeOfReturningArray)
{
    int currentArrayLength = getEffectiveArrayLength(id, ID_ARRAY_SIZE);
    *sizeOfReturningArray = currentArrayLength;
    return id;
}

void deleteUser_checkCardID(void)
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
        for(;i<MAX_CARD_NUMBER;i++)
        {
        	numero[i]=-1;
        }
        int8_t * idptr = getIdByCardNumber(numero);
        if(idptr != NULL && removeUser(idptr) == DELETE_SUCCESSFULL)
        {
        	emitEvent(ID_OK_EV);
        }
        else
        {
            idFail();   //The ID is invalid or doesn't exist.
        }
    }
    else
    {
        idCardFail();   //The lector failed to read the card.
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
    SevenSegDisplay_WriteBufferAndMove("ID OR CARD", 10, 0, BOUNCE);
    showingTitle = true;
    titleTimerID = Timer_AddCallback(&stopShowingTitle, TITLE_TIME, true);  //Starts the callback to stop the title.
}

static void stopShowingTitle(void)
{
    SevenSegDisplay_EraseScreen();
    SevenSegDisplay_SetPos(0);
    SevenSegDisplay_WriteBuffer((char *)id, ID_ARRAY_SIZE, 0);  //Initializes the input.
    SevenSegDisplay_CursorOn();
    showingTitle = false;
}

static void userInteractionStopsTitle(void)
{
    Timer_Delete(titleTimerID);
    titleTimerID = -1;
    stopShowingTitle();
}

static void userInteractionStopErrorIndicationAndRestart(void)
{
    userInteractionStopsErrorIndication();
    deleteUser_initState(); //Restarts the state cycle.
}

static void userInteractionStopsErrorIndication(void)
{
    SevenSegDisplay_EraseScreen();
    Timer_Delete(errorIndicationTimerID);   //Cancels the callback.
    showingErrorIndication = false;
    errorIndicationTimerID = -1;
}

static void idFail(void)
{
    SevenSegDisplay_EraseScreen();
    SevenSegDisplay_CursorOff();
    SevenSegDisplay_SetPos(0);
    SevenSegDisplay_WriteBufferAndMove("NO ID FOUND", 11, 0, BOUNCE);   //Shows error indication message.
    showingErrorIndication = true;
    errorIndicationTimerID = Timer_AddCallback(&deleteUser_initState, TITLE_TIME, true);    //Starts the callback to restart the state's cycle.
}

static void idCardFail(void)
{
    SevenSegDisplay_EraseScreen();
    SevenSegDisplay_CursorOff();
    SevenSegDisplay_SetPos(0);
    SevenSegDisplay_WriteBufferAndMove("CARD FAILED", 11, 0, BOUNCE); //Shows error indication message.
    showingErrorIndication = true;
    errorIndicationTimerID = Timer_AddCallback(&deleteUser_initState, TITLE_TIME, true);     //Starts the callback to restart the state's cycle.
}
