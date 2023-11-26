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
#include <os.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
enum cardStatus
{
	CARD_SUCCESS,
	CARD_FAIL,
	CARD_IDLE
};

#define	SS	0b0001011
#define	FS	0b0001101
#define	ES_	0b0011111

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
void cardReader_Init(void);

/**
 * @brief Initialize CardReader driver
 * @return 0 if the data is ready, 1 if the data is wrong, 2 if no data was obtained.
 */
uint8_t getCardReader_Status(void);

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

/**
 * @brief Get the card ID
 * @param data_buffer Pointer to an array that stores the data from the card (at least 40 bytes)
 * @param number_of_characters	Size of the data_buffer (should be at most 40)
 * @param ID_buffer Pointer to an array that stores the data from the card (at least 9 bytes)
 * @return True if data could be successfully read, False otherwise
 */
bool getCard_ID(uint8_t *data_buffer, uint8_t number_of_characters, uint8_t *ID_buffer);

/*******************************************************************************
 ******************************************************************************/

#endif // _CARDREADER_DRV_H_
