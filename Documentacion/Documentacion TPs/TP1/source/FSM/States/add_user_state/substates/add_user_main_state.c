/***************************************************************************/ /**
  @file     add_user_main_menu_state.c
  @brief    Main welcome state functions.
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "add_user_main_state.h"
#include "lector.h"
#include "data_base.h"//:-)
#include "queue.h"
#include "seven_seg_display.h"
#include "Timer.h"

/******************************************************************************
 *  CONSTANTS
 ******************************************************************************/
#define TITLE_TIME 8000

/*******************************************************************************
*       VARIABLE WITH LOCAL SCOPE
*******************************************************************************/
static int8_t cardNumber[MAX_CARD_NUMBER_LEN];
static int errorIndicationTimerID = -1;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
/**
 * @brief Show the title of the state in the display. If the user interacts with the system, the title will stop showing and the input will start.
 */
static void showTitle(void);
/**
 * @brief Function executed when the card is already used.
 */
static void cardIsUsed(void);
/**
 * @brief Function executed when the lector fails to read the card.
 */
static void CardFail(void);
/*******************************************************************************
 * FUNCTIONS WITH GLOBAL SCOPE
 ******************************************************************************/
void addUsr_initState(void)
{
    //showingErrorIndication = false;
    showTitle();
    uint8_t i;
    for (i = 0; i< MAX_CARD_NUMBER_LEN; i++)
    {
        cardNumber[i] = -1;
    }  
}


void addUsr_cardSwipe(void)
{
    card_t myCard;
    bool ok = Lector_GetData(&myCard);
    if(ok)
    {
        uint8_t i,length = myCard.number_len;
        for(i = 0; i<length; i++)
        {
            cardNumber[i] = myCard.card_number[i];
        }
        
        if(!verifyCardNumber(cardNumber, length))
        {
            emitEvent(ID_OK_EV);
        }
        else
        {
            cardIsUsed();
            for (i = 0; i< MAX_CARD_NUMBER_LEN; i++)
                cardNumber[i] = -1;
        }
    }
    else
    {
        CardFail();
    }
}

int8_t * addUsr_getCardNumber(void)
{
    return cardNumber;
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
    SevenSegDisplay_WriteBufferAndMove("PASS CARD", 9, 0, BOUNCE);
}

static void CardFail(void)
{
    SevenSegDisplay_EraseScreen();
    SevenSegDisplay_CursorOff();
    SevenSegDisplay_SetPos(0);
    SevenSegDisplay_WriteBufferAndMove("CARD FAILED", 11, 0, SHIFT_L);
    errorIndicationTimerID = Timer_AddCallback(&addUsr_initState, TITLE_TIME, true);
}

static void cardIsUsed(void)
{
    SevenSegDisplay_EraseScreen();
    SevenSegDisplay_CursorOff();
    SevenSegDisplay_SetPos(0);
    SevenSegDisplay_WriteBufferAndMove("CARD IS USED", 12, 0, SHIFT_L);
    errorIndicationTimerID = Timer_AddCallback(&addUsr_initState, TITLE_TIME, true);
}

