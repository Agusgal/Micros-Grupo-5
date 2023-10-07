/***************************************************************************//**
  @file     SPI.h
  @brief    SPI driver
  @author   Grupo 5
 ******************************************************************************/

#ifndef SPI_H_
#define SPI_H_

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
 * @brief
 * @return
 */
void SPI_Init (void);

/**
 * @brief
 * @return
 */
uint8_t SPI_Get_Status(void);

/**
 * @brief
 * @return
 */
uint8_t SPI_Get_Data(void);

/**
 * @brief	Starts the Transmission of the data (8-bits words)
 * @param	bytes	Array of data (uint8_t*)
 * @param	num_of_bytes	Number of bytes of the array
 */

void SPI_SendData(uint8_t* bytes, uint32_t num_of_bytes);


/**
 * @brief	Starts the Transmission of the string (8-bits words). The '\0' is not transmitted
 * @param	msg	Array of data (string)
 */

void SPI_SendMsg(uint8_t* msg);

/**
 * @brief	Starts the Transmission of the only byte (8-bits word).
 * @param	byte	Data byte to transmit.
 */
void SPI_SendByte(uint8_t byte);

/**
 * @return	Transmission in process (0 No tranmission; 1 in process)
 */
uint8_t SPI_Transmission_In_Process();


/*******************************************************************************
 ******************************************************************************/

#endif /* SPI_H_ */
