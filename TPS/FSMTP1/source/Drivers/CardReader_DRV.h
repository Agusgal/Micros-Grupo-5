/***************************************************************************//**
  @file     CardReader_DRV.h
  @brief    Card Reader driver
  @author   Grupo 5
 ******************************************************************************/

#ifndef _CARDREADER_DRV_H_
#define _CARDREADER_DRV_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/



/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Initialize CardReader driver
 * @return Initialization succeed
 */
bool cardReader_Init(void);

/**
 * @brief Initialize CardReader driver
 * @return 2 if the data is ready, 1 if the data is being read, 0 if no data was obtained.
 */
uint8_t getCardReader_Status(void);

/**
 * @brief Get the card data
 * @param data Pointer to an array that will store the data from the card
 * @return True if data could be succesfully read, False otherwise
 */
bool getCardReader_Data(uint8_t (*data) [40]);

/*******************************************************************************
 ******************************************************************************/

#endif // _CARDREADER_DRV_H_
