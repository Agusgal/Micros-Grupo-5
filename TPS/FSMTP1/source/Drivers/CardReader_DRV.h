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
enum cardState
{
	IDLE,
	READING,
	FINISH
};

#define	SS	0b0001011U
#define	FS	0b0001101U
#define	ES	0b0011111U

#define NUMBER_OF_CHARACTERS	40

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
 * @return True if the data is ready, False if not ready.
 */
bool getCardReader_Status(void);

/**
 * @brief Get the card data
 * @param data Pointer to an array that will store the data from the card (at least 40 bytes)
 */
void getCardReader_Data(uint8_t *data_buffer);

/**
 * @brief Check if data is valid with the LRC
 * @param data_buffer Pointer to an array that stores the data from the card (at least 40 bytes)
 * @param number_of_characters	Size of the data_buffer (should be at most 40)
 * @return True if data could be successfully read, False otherwise
 */
bool checkLRC_CardReader_Data(uint8_t *data_buffer, uint8_t number_of_characters);

/*******************************************************************************
 ******************************************************************************/

#endif // _CARDREADER_DRV_H_
