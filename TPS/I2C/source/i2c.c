
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "i2c.h"
#include "MK64F12.h"
#include "board.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/



static void finish_com (I2C_FAULT fault);

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


void I2C0_IRQHandler(void)
{

}


void finish_com (I2C_FAULT fault)
{

}



/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/




void i2cInit (I2C_Modules module)
{
	I2C_Type *i2c_p_array [] = I2C_BASE_PTRS;
	I2C_Type *i2c_p = i2c_p_array[module];

	IRQn_Type i2c_irq_array[] = I2C_IRQS;
	IRQn_Type i2c_irq = i2c_irq_array[module];


	//Clock gating modulo I2C
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK | SIM_SCGC4_I2C1_MASK;
	SIM->SCGC1 |= SIM_SCGC1_I2C2_MASK;

	//Clock gating puertos
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;


	//Enable I2C control register del modulo y de sus interrupciones
	i2c_p->C1 |= I2C_C1_IICEN_MASK | I2C_C1_IICIE_MASK;

	//Status register para accion inicial
	i2c_p->S = I2C_S_TCF_MASK | I2C_S_IICIF_MASK;


	//Setear baudrate TODO: hacer una funcion para configurar baudrate custom
	i2c_p->F = I2C_F_MULT(0) | I2C_F_ICR(0); //  set the I2C baud rate


	//Configuracion de interrupcion
	NVIC_EnableIRQ(i2c_irq);

	//Mux puertos en modo I2C


}


void i2cWriteMsg(I2C_COM_CONTROL * i2c_comm)
{

}




I2C_FAULT i2cWriteMsgBlocking (uint8_t * msg, uint8_t data_size,	uint8_t register_address, uint8_t slave_address )
{

}




I2C_FAULT i2cReadMsgBlocking (uint8_t * buffer, uint8_t data_size,	uint8_t register_address, uint8_t slave_address )
{


}
