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
void UART_SetBaudRate (UART_Type *uart, uint32_t baudrate);

/**
 * @brief
 * @return
 */
unsigned char UART_Get_Status(uint8_t id);

/**
 * @brief
 * @return
 */
unsigned char UART_Get_Data(uint8_t id);

/**
 * @brief
 * @return
 */
void UART_Send_Data(unsigned char txdata);

/**
 * @brief
 * @return
 */
void UART_SendMsg(char* msg, uint8_t id);

/**
 * @brief
 * @return
 */
void UART_SendChar(char msg, uint8_t id);


/*******************************************************************************
 ******************************************************************************/

#endif /* SPI_H_ */
