/*
 * i2c.h
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


#define I2C_BUS_CLOCK	50000000U
#define ADDRESS_CYCLE_BYTES 2
#define NULL 	0

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef void (* pfunc) (void);

//TODO: ver si se puede hacer de otra manera esto
typedef enum {I2C0_M, I2C1_M, I2C2_M, I2C_M_Count} I2C_Module_t;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
typedef enum{
	I2C_Read,
	I2C_Write,
}I2C_Mode_t;

// Al necesitar un ACK, los estados definen que acción se realizó, para la cual se está esperando un acknowledge
typedef enum{
	I2C_Idle,
	I2C_Address_RW,
	I2C_Read_Dummy,
	I2C_Read_Byte,
	I2C_Write_Byte,
	I2C_Fail
} I2C_Status_t;


typedef uint8_t I2C_Address_t;

typedef struct{
	I2C_Status_t status;
	I2C_Mode_t mode;
	uint8_t * read_buffer;
	size_t read_size;
	uint8_t data;
	uint8_t * write_buffer;
	size_t write_size;
	I2C_Address_t slave_address;
	I2C_Address_t slave_reg;
	size_t RW_index;
}I2C_Object_t;


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


/**
 * @brief Initialize i2c driver
 * @param id i2c's number
 * @param config i2c's configuration (baudrate, parity, etc.)
*/
void I2C_InitModule (I2C_Module_t module);


I2C_Status_t I2C_InitObject(I2C_Module_t module, I2C_Mode_t mode, uint8_t * read_buffer, size_t read_size, uint8_t * write_buffer,
					size_t write_size, I2C_Address_t slave_address,I2C_Address_t slave_reg);

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

I2C_Status_t I2C0_Com (uint8_t * read_buffer, size_t read_size, uint8_t * write_buffer,
		size_t write_size, I2C_Address_t slave_address,I2C_Address_t slave_reg);


/*******************************************************************************
 ******************************************************************************/


#endif /* I2C_H_ */
