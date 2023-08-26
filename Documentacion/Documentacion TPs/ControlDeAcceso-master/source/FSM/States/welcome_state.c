/***************************************************************************/ /**
  @file     main_menu_state.c
  @brief    Main welcome state functions.
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <States/welcome_state.h>
#include "lector.h"
#include "data_base.h"
#include "queue.h"
#include "seven_seg_display.h"
#include "./drivers/Led.h"
#include "Led.h"
#include "MplxLed.h"
/*******************************************************************************
 * FUNCTIONS WITH GLOBAL SCOPE
 ******************************************************************************/
void showWelcomeAnimation(void)
{
	Led_StopAllProcessedFromAllLeds();
	MplxLed_StopAllProcessedFromAllLeds();

    SevenSegDisplay_EraseScreen();
    SevenSegDisplay_CursorOff();
    SevenSegDisplay_SetPos(0);
    SevenSegDisplay_WriteBufferAndMove("  Hi  ", 6, 0, BOUNCE);
}

void cardSwipe(void)
{
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
            emitEvent(ID_FAIL_EV);
        }
    }
    else
    {
        emitEvent(CARD_FAIL_EV);
    }
}
