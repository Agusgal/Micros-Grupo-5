/***************************************************************************/ /**
  @file     user_input.h
  @brief    User input header
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/
#ifndef USER_INPUT_H_
#define USER_INPUT_H_
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define NO_INPUT_CHAR -1
#define BACKSPACE -2
#define MID_LINE -3
//CHAR ORDER: 0 1 2 3 4 5 6 7 8 9 BACKSPACE

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Changes the current position of the array to the next character following CHAR ORDER.
 * @param inputArray: The array of characters introduced by the user.
 * @param currentPosition: The value of the iterator which stores the current position of the user in the input array.
 */
void inputIncreaseCurrent(int8_t *inputArray, uint8_t currentPosition);
/**
 * @brief Changes the current position of the array to the previous character following CHAR ORDER.
 * @param inputArray: The array of characters introduced by the user.
 * @param currentPosition: The value of the iterator which stores the current position of the user in the input array.
 */
void inputDecreaseCurrent(int8_t *inputArray, uint8_t currentPosition);
/**
 * @brief Set the current position of the array with the introduced character and moves to the next position (unless the character is equal to BACKSPACE).
 *        If the introduced character is equal to BACKSPACE, it deletes the character of the current position and moves to the previous one.
 * @param inputArray: The array of characters introduced by the user.
 * @param currentPosition: A pointer to the iterator which stores the current position of the user in the input array.
 * @param totalArraySize: The previosly fixed size of the inputArray.
 */
void inputAcceptNumber(int8_t *inputArray, uint8_t *currentPosition, int totalArraySize);

/**
 * @brief Function executed to initialize the input array..
 * @param inputArray: The array of characters introduced by the user.
 * @param currentPosition: A pointer to the iterator which stores the current position of the user in the input array.
 * @param totalArraySize: The previosly fixed size of the inputArray.
 */
void inputResetArray(int8_t *inputArraey, uint8_t *currentPosition, int totalArraySize);

/**
 * @brief Calculates the number of characters introduced by the user in the input array.
 * @param inputArray: Teh array of characters introduced by the user.
 * @param totalArraySize: The previosly fixed size of the inputArray.
 * @return The amount of elements different from NO_INPUT_CHAR and BACKSPACE in the input array (its effective length). The elements must be consecutive
 */
int getEffectiveArrayLength(int8_t *inputArray, int totalArraySize);


#endif //USER_INPUT_H_
