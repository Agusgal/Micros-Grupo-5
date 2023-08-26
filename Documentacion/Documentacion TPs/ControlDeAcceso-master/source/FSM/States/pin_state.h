/***************************************************************************/ /**
  @file     pin_state.h
  @brief    Pin state header
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/
#ifndef PIN_H_
#define PIN_H_
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
void initPinInput(void);
/**
 * @brief Changes the current position of the PIN array to the next character following CHAR ORDER.
 */
void pin_increaseCurrent(void);
/**
 * @brief Changes the current position of the PIN array to the previous character following CHAR ORDER.
 */
void pin_decreaseCurrent(void);
/**
 * @brief Set the current position of the PIN array with the introduced character and moves to the next position (unless the character is equal to BACKSPACE).
 *        If the introduced character is equal to BACKSPACE, it deletes the character of the current position and moves to the previous one.
 */
void pin_acceptNumber(void);
/**
 * @brief Checks if the entered PIN is correct and corresponds to an user or an admin in the database.
 *        Adds a PIN_OK or a PIN_FAIL event to the event queue of the FSM.
 */
void pin_confirmPin(void);

/**
 * @brief Returns the array of the Pin.
 * @param sizeOfReturningArray: A pointer to an integer where the size of the pin array gets stored.
 * @return The array of introduced characters.
 */
int8_t *pin_getPinArray(int *sizeOfReturningArray);
#endif //PIN_H_
