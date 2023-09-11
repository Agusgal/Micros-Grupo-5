/***************************************************************************/ /**
  @file     add_user_main_menu_state.h
  @brief    Add user Main welcome state header
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/
#ifndef ADD_USER_MAIN_MENU_STATE_H
#define ADD_USER_MAIN_MENU_STATE_H

#include <stdint.h>

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void addUsr_initState(void);
void addUsr_cardSwipe(void);
int8_t * addUsr_getCardNumber(void);
#endif /* ADD_USER_MAIN_MENU_STATE_H */
