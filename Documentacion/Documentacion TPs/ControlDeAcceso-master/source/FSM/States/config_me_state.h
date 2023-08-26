/*******************************************************************************
  @file     config_me_state.h
  @brief    Config me state header
  @author   Grupo 2 - Lab de Micros
 **********/
#ifndef CONFIG_ME_H
#define CONFIG_ME_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>

//CHAR ORDER: 0 1 2 3 4 5 6 7 8 9 BACKSPACE

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Initialization of the state.
 */
void configMe_initState(void);
/**
 * @brief Changes the current position of the PIN array to the next character following CHAR ORDER.
 */
void configMe_increaseCurrent(void);
/**
 * @brief Changes the current position of the PIN array to the previous character following CHAR ORDER.
 */
void configMe_decreaseCurrent(void);
/**
 * @brief Set the current position of the PIN array with the introduced character and moves to the next position (unless the character is equal to BACKSPACE).
 *        If the introduced character is equal to BACKSPACE, it deletes the character of the current position and moves to the previous one.
 */
void configMe_acceptNumber(void);
/**
 * @brief Function called when the user confirms the new ID.
 */
void configMe_confirmPin(void);


#endif //CONFIG_ME_H