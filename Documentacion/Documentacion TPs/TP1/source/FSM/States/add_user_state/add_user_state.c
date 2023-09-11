/***************************************************************************/ /**
  @file     add_user.c
  @brief    add user state functions
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "add_user_state.h"
#include <stdbool.h>
#include "data_base.h"
#include "queue.h"
#include "../user_input.h"
#include "add_user_fsm_table.h"
/*******************************************************************************
 * GLOBAL VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static STATE *currentState;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 **********************************************************************/

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void addUser_initFSM(void)
{
    currentState = subFSM_GetInitState();
    subFSM_StartInitState();
}

void addUser_finishConfiguration(void)
{
    //emitEvent(ADD_USER_FINISHED_EV);
    
}

void addUser_onLKP(void)
{
    currentState = fsm(currentState, LKP_EV);
}
void addUser_onPress(void)
{
    currentState = fsm(currentState, PRESS_EV);
}
void addUser_onEncoderRight(void)
{
    currentState = fsm(currentState, ENCODER_RIGHT_EV);
}
void addUser_onEncoderLeft(void)
{
    currentState = fsm(currentState, ENCODER_LEFT_EV);
}

void addUser_onCardSwipe(void)
{
    currentState = fsm(currentState, CARD_SWIPE_EV);
}

void addUser_onReturn(void)
{
    currentState = fsm(currentState, RETURN_TO_LAST_STATE_EV);
}

void addUser_onIdOk(void)
{
    currentState = fsm(currentState, ID_OK_EV);
}

void addUser_onIdFail(void)
{
    currentState = fsm(currentState, ID_FAIL_EV);
}

void addUser_onCardFail(void)
{
    currentState = fsm(currentState, CARD_FAIL_EV);
}
