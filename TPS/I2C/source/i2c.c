
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "i2c.h"
#include "MK64F12.h"
#include "board.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define I2C_START_SIGNAL        	(I2C->C1 |= I2C_C1_MST_MASK) //generates start signal
#define I2C_STOP_SIGNAL          	(I2C->C1 &= ~I2C_C1_MST_MASK)//generetes stop signal
#define I2C_REPEAT_START_SIGNAL 	(I2C->C1 |= I2C_C1_RSTA_MASK) //generetes repeated start signal
#define I2C_WRITE_DATA(data)    	(I2C->D = data) //Write data for transfer
#define I2C_READ_DATA           	(I2C->D)
#define I2C_GET_IRQ_FLAG        	(I2C->S & I2C_S_IICIF_MASK)
#define I2C_CLEAR_IRQ_FLAG      	(I2C->S |= I2C_S_IICIF_MASK)
#define I2C_GET_RX_ACK				(!(I2C->S & I2C_S_RXAK_MASK))
#define I2C_SET_RX_MODE         	(I2C->C1 &= ~I2C_C1_TX_MASK)
#define I2C_SET_TX_MODE         	(I2C->C1 |= I2C_C1_TX_MASK)
#define I2C_SET_NACK	         	(I2C->C1 |= I2C_C1_TXAK_MASK)
#define I2C_CLEAR_NACK      	 	(I2C->C1 &= ~I2C_C1_TXAK_MASK)
#define I2C_CLEAR_NACK     			(I2C->C1 &= ~I2C_C1_TXAK_MASK)
#define I2C_CHECK_BUS		  		(I2C->S & I2C_S_BUSY_MASK)
#define I2C_GET_TCF					(I2C->S & I2C_S_TCF_MASK)
#define I2C_CLEAR_TCF            	(I2C->S |= I2C_S_TCF_MASK)
#define I2C_RW_MASK					(0xFF & I2C_Objects[module].mode)
#define I2C_ADDRESS_MASK			((I2C_Objects[module].slave_address & 0x7F) << 1)
#define I2C_ADRESS_RW_BYTE			(I2C_ADDRESS_MASK & I2C_RW_MASK)

#define I2C_OBJ_MODE				(I2C_Objects[module].mode)
#define I2C_OBJ_STATUS				(I2C_Objects[module].status)
#define I2C_OBJ_SLAVE_ADDR			(I2C_Objects[module].slave_address)
#define I2C_OBJ_RW_INDEX			(I2C_Objects[module].RW_index)
#define I2C_OBJ_R_BUFFER			(I2C_Objects[module].read_buffer)
#define I2C_OBJ_R_SIZE				(I2C_Objects[module].read_size)
#define I2C_OBJ_W_BUFFER			(I2C_Objects[module].write_buffer)
#define I2C_OBJ_W_SIZE				(I2C_Objects[module].write_size)
/*******************************************************************************
 * VARIABLE PROTOTYPES WITH LOCAL SCOPE
 ******************************************************************************/

I2C_Type * I2C_array [] = I2C_BASE_PTRS;


typedef enum{
	I2C_Write,
	I2C_Read,
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

typedef uint32_t I2C_Address_t;

typedef struct{
	I2C_Status_t status;
	I2C_Mode_t mode;
	uint8_t * read_buffer;
	size_t read_size;
	uint8_t * write_buffer;
	size_t write_size;
	I2C_Address_t slave_address;
	size_t RW_index;
}I2C_Object_t;

static I2C_Object_t I2C_Objects[I2C_M_Count];

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void I2C_InitObject(I2C_Module_t module, I2C_Mode_t mode, uint8_t * read_buffer, size_t read_size, uint8_t * write_buffer,
					size_t write_size, I2C_Address_t slave_address);
void finish_com (void)
{

}



/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/




void I2CInitModule (I2C_Module_t module)
{

	I2C_Type * I2C = I2C_array[module];

	IRQn_Type I2C_IRQ_array[] = I2C_IRQS;
	IRQn_Type I2C_IRQ = I2C_IRQ_array[module];


	//Clock gating modulo I2C
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK | SIM_SCGC4_I2C1_MASK;
	SIM->SCGC1 |= SIM_SCGC1_I2C2_MASK;

	//Clock gating puertos
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;


	//Enable I2C control register del modulo y de sus interrupciones
	I2C->C1 |= I2C_C1_IICEN_MASK | I2C_C1_IICIE_MASK;

	//Status register para accion inicial
	I2C->S = I2C_S_TCF_MASK | I2C_S_IICIF_MASK;


	//Setear baudrate TODO: hacer una funcion para configurar baudrate custom
	I2C->F = I2C_F_MULT(0) | I2C_F_ICR(0); //  set the I2C baud rate


	//Configuracion de interrupcion
	NVIC_EnableIRQ(I2C_IRQ);

	//Mux puertos en modo I2C
}



/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


void I2C_InitObject(I2C_Module_t module, I2C_Mode_t mode, uint8_t * read_buffer, size_t read_size, uint8_t * write_buffer,
					size_t write_size, I2C_Address_t slave_address)
{
	// Set parameters for RAM I2C_Object

	I2C_OBJ_MODE=mode;
	I2C_OBJ_SLAVE_ADDR=slave_address;
	I2C_OBJ_STATUS=I2C_Idle;
	I2C_OBJ_R_BUFFER=read_buffer;
	I2C_OBJ_R_SIZE=read_size;
	I2C_OBJ_W_BUFFER=write_buffer;
	I2C_OBJ_W_SIZE=write_size;

	// Si se requieren leer o escribir 0 bytes, Fail.
	if ( (mode==I2C_Write && !write_size) || (mode==I2C_Read && !read_size) )
	{
		I2C_OBJ_STATUS=I2C_Fail;
		return;
	}

	// I2C_Type Instance of selected I2C
	I2C_Type * I2C = I2C_array[module];

	// Initialize communication
	if (I2C_CHECK_BUS)		//
	{
		I2C_SET_TX_MODE;	// Set to transmit mode
		I2C_START_SIGNAL;	// Start Signal (Setting Master Mode)
		I2C_WRITE_DATA(I2C_ADRESS_RW_BYTE);
		I2C_OBJ_STATUS=I2C_Address_RW;	//	Se pasa al estado I2C_Address_RW, esperando su acknowledge (en la interrupción)
		I2C_OBJ_RW_INDEX=0;
	}
	else
	{
		I2C_OBJ_STATUS=I2C_Fail;
		return;
	}

}

void I2C_IRQHandler(I2C_Module_t module)
{

	I2C_Type * I2C = I2C_array[module];
	I2C_CLEAR_IRQ_FLAG;
	switch(I2C_OBJ_MODE)
	{
		case I2C_Write:
		// Etapa de escritura. Ya fueron escritos el Start Signal, y el Address + W en I2C_InitObject()
			switch(I2C_OBJ_STATUS)
			{

				case I2C_Address_RW:
					if(I2C_GET_RX_ACK)	// Recibió un acknowledge del Address + W
					{
						I2C_OBJ_STATUS=I2C_Write_Byte;	// Paso a escribir el primer byte
						I2C_WRITE_DATA(I2C_OBJ_W_BUFFER[I2C_OBJ_RW_INDEX]);
						I2C_OBJ_RW_INDEX++;

					}
					else
						I2C_OBJ_STATUS=I2C_Fail;
					break;

				case I2C_Write_Byte:
				{
					if (I2C_GET_RX_ACK)	// Recibió bien el byte escrito (incluido el primer byte enviado en la etapa anterior)
					{
						if(I2C_OBJ_RW_INDEX==I2C_OBJ_W_SIZE) // Si ya no quedan bytes por escribir
						// Ver si ahora quiero leer el slave, sino termino la comunicación
						{
							if (I2C_OBJ_R_SIZE)
							// Si hay algo para leer, inicializo el frame de lectura enviando un repeat start y el address+R
							{
								I2C_OBJ_STATUS=I2C_Address_RW;
								I2C_OBJ_RW_INDEX=0;
								I2C_OBJ_MODE=I2C_Read;
								I2C_REPEAT_START_SIGNAL;
								I2C_WRITE_DATA(I2C_ADRESS_RW_BYTE);

							}
							else	// Si no hay nada, se terminó la comunicación y libero el bus.
							{
								I2C_OBJ_STATUS=I2C_Idle;
								I2C_STOP_SIGNAL;
							}
						}

						else	// Si quedan algunos bytes por escribir, los escribo
						{
							I2C_WRITE_DATA(I2C_OBJ_W_BUFFER[I2C_OBJ_RW_INDEX]);
							I2C_OBJ_RW_INDEX++;
						}
					}
					else	// No recibió el acknowledge
						I2C_OBJ_STATUS=I2C_Fail;
				}
				break;	// I2C_Write_Byte

			}
			break;	// I2C_Write

		case I2C_Read:
		// Etapa de lectura. Ya fueron escritos el Start Signal, y el Address + R en I2C_InitObject(), o en el pasaje de Write a Read
			switch (I2C_OBJ_STATUS)
			{
			case I2C_Address_RW:
				if (I2C_GET_RX_ACK)		// Si el address fue Acknowledged, hago la dummy read y seteo modo Receive.
				{
					I2C_OBJ_STATUS=I2C_Read_Dummy;
					I2C_SET_RX_MODE;
					uint8_t dummy_read = I2C_READ_DATA;
				}
				else
					I2C_OBJ_STATUS=I2C_Fail;
				break;
			case I2C_Read_Dummy:	// Ya realizó el dummy read
				if(I2C_OBJ_RW_INDEX == I2C_OBJ_R_SIZE-1) // Si tengo que leer un único dato
				{
					I2C_SET_NACK;
					I2C_STOP_SIGNAL;
					I2C_OBJ_STATUS=I2C_Idle;
				}
				else
				{
					I2C_OBJ_STATUS=I2C_Read_Byte;
				}
				I2C_OBJ_R_BUFFER[I2C_OBJ_RW_INDEX] = I2C_READ_DATA;
				I2C_OBJ_RW_INDEX++;
				break;
			case I2C_Read_Byte:
				if(I2C_OBJ_RW_INDEX == I2C_OBJ_R_SIZE-1) // Si estoy leyendo el último dato
				{
					I2C_SET_NACK;			// Desabilito el Transmit acknowledge para cortar la lectura
					I2C_STOP_SIGNAL;		// Emito la señal de stop
					I2C_CLEAR_NACK;			// Restablezco el Transmit acknowledge
					I2C_OBJ_R_BUFFER[I2C_OBJ_RW_INDEX] = I2C_READ_DATA;	//Leo el último dato
					I2C_OBJ_STATUS=I2C_Idle;
				}
				else	// Si no es el último, sigo leyendo
				{
					I2C_OBJ_R_BUFFER[I2C_OBJ_RW_INDEX] = I2C_READ_DATA;
					I2C_OBJ_RW_INDEX++;
				}
				break;
			}
			break; // I2C_Read
		}
}



void I2C0_IRQHandler(void)
{
	I2C_IRQHandler(I2C0_M);
}
void I2C1_IRQHandler(void)
{
	I2C_IRQHandler(I2C1_M);
}

void I2C2_IRQHandler(void)
{
	I2C_IRQHandler(I2C2_M);
}
