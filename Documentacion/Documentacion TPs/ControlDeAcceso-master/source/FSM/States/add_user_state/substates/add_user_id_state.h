/***************************************************************************/ /**
  @file     add_user_id_state.h
  @brief    ID state header
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/
#ifndef ADD_USER_ID_H_
#define ADD_USER_ID_H_
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>

//CHAR ORDER: 0 1 2 3 4 5 6 7 8 9 BACKSPACE

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Initializes the input of the user.
 */
void addUserId_initLogin(void);

/**
 * @brief Changes the current position of the ID array to the next character following CHAR ORDER.
 */
void addUserId_increaseCurrent(void);
/**
 * @brief Changes the current position of the ID array to the previous character following CHAR ORDER.
 */
void addUserId_decreaseCurrent(void);
/**
 * @brief Set the current position of the ID array with the introduced character and moves to the next position (unless the character is equal to BACKSPACE).
 *        If the introduced character is equal to BACKSPACE, it deletes the character of the current position and moves to the previous one.
 */
void addUserId_acceptNumber(void);
/**
 * @brief Checks if the entered ID is correct and corresponds to an user or an admin in the database.
 *        Adds a ID_OK or a ID_FAIL event to the event queue of the FSM.
 */
void addUserId_confirmID(void);

/**
 * @brief Returns the array of the ID.
 * @param sizeOfReturningArray: A pointer to an integer where the size of the ID array gets stored.
 * @return The array of introduced characters.
 */
int8_t* addUserId_getIDArray(void);



#endif //ADD_USER_ID_H_