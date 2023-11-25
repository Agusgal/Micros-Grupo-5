/***************************************************************************//**
  @file     IoT.h
  @brief	IoT cloud comunication services
  @author	Grupo 5
  @date		23 nov. 2023
 ******************************************************************************/

#ifndef _IOT_H_
#define _IOT_H_

#include <stdint.h>

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Initialize IOT module
*/
void IoT_Init();


/**
 * @brief Update floor state
 * @param f_i: Number of people in the i-th floor
*/
void IoT_Update(uint16_t f1, uint16_t f2, uint16_t f3);


/*******************************************************************************
 ******************************************************************************/

#endif // _IOT_H_
