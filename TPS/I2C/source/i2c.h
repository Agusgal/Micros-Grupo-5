/*
 * i2c.h
 *
 *  Created on: Sep 16, 2019
 *      Author: martinamaspero
 */

#ifndef I2C_H_
#define I2C_H_


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>




/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



#define ADDRESS_CYCLE_BYTES 2

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef void (* pfunc) (void);

//TODO: ver si se puede hacer de otra manera esto
typedef enum {I2C0_M, I2C1_M, I2C2_M, I2C_M_Count} I2C_Module_t;


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


/**
 * @brief Initialize i2c driver
 * @param id i2c's number
 * @param config i2c's configuration (baudrate, parity, etc.)
*/
void I2CInitModule (I2C_Module_t module);


/**
 * @brief Read a received message. Non-Blocking
 * @param id i2c's number
 * @param msg Buffer to paste the received bytes
 * @param cant Desired quantity of bytes to be pasted
 * @return Real quantity of pasted bytes
*/
void i2cReadMsg(void);


/**
 * @brief Write a message to be transmitted. Non-Blocking
 * @param id i2c's number
 * @param msg Buffer with the bytes to be transfered
 * @param cant Desired quantity of bytes to be transfered
 * @return Real quantity of bytes to be transfered
*/
void i2cWriteMsg(I2C_Module_t module);




/*******************************************************************************
 ******************************************************************************/


#endif /* I2C_H_ */
