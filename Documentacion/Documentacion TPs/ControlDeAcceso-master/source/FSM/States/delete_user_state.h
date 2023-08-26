/***************************************************************************/ /**
  @file     delete_user_state.h
  @brief    delete user state header
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/
#ifndef DELETE_USER_H
#define DELETE_USER_H

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
void deleteUser_initState(void);
/**
 * @brief Changes the current position of the PIN array to the next character following CHAR ORDER.
 */
void deleteUser_increaseCurrent(void);
/**
 * @brief Changes the current position of the PIN array to the previous character following CHAR ORDER.
 */
void deleteUser_decreaseCurrent(void);
/**
 * @brief Set the current position of the PIN array with the introduced character and moves to the next position (unless the character is equal to BACKSPACE).
 *        If the introduced character is equal to BACKSPACE, it deletes the character of the current position and moves to the previous one.
 */
void deleteUser_acceptNumber(void);
/**
 * @brief Function called when the user confirms the new ID.
 */
void deleteUser_confirm(void);
/**
 * @brief Function called when card is swiped.
 */
void deleteUser_checkCardID(void);

/**
 * @brief returns a pointer to the id array
 */
int8_t * deleteUser_getIdArray(int *sizeOfReturningArray);


#endif //DELETE_USER_H