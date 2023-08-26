/***************************************************************************/ /**
  @file     add_user_state.h
  @brief    add user state header.
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

#ifndef ADD_USER_STATE_H_
#define ADD_USER_STATE_H_

#include "../../fsm.h"

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Function to be called when the adduration ends. It emits USER_ADD_USER_FINISHED_EV and ADMIN_ADD_USER_FINISHED_EV events to the event queue.
 */
void addUser_initFSM(void);
void addUser_finishConfiguration(void);


void addUser_onLKP(void);
void addUser_onPress(void);
void addUser_onEncoderRight(void);
void addUser_onEncoderLeft(void);
void addUser_onCardSwipe(void);
void addUser_onReturn(void);
void addUser_onIdOk(void);
void addUser_onCardFail(void);
void addUser_onIdFail(void);

#endif /* ADD_USER_STATE_H_*/
