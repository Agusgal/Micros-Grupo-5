/***************************************************************************/ /**
  @file     menu_state.h
  @brief    menu state
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/
#ifndef ADMIN_STATE_H
#define ADMIN_STATE_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Initializes the welcome
 */
void menu_initState(void);
/**
 * @brief Changes the current position of the ID array to the next character following CHAR ORDER.
 */
void menu_nextOption(void);
/**
 * @brief Shows the next welcome option.
 */
void menu_previousOption(void);

/**
 * @brief Selects the current option. Adds an event to the event queue of the FSM.
 */
void menu_selectOption(void);

#endif /* ADMIN_STATE_H */