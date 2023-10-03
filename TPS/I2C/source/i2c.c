
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "i2c.h"
#include "MK64F12.h"
#include "board.h"
#include "hardware.h"
#include "gpio.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



// I2C OnBoard Pinout
#define I2C0_SCL_PIN				24
#define I2C0_SDA_PIN				25

// I2C Register Macros
#define I2C_START_SIGNAL        	(I2C_array[module]->C1 |= I2C_C1_MST_MASK) //generates start signal
#define I2C_STOP_SIGNAL          	(I2C_array[module]->C1 &= ~I2C_C1_MST_MASK)//generetes stop signal
#define I2C_REPEAT_START_SIGNAL 	(I2C_array[module]->C1 |= I2C_C1_RSTA_MASK) //generetes repeated start signal
#define I2C_WRITE_DATA(data)    	(I2C_array[module]->D = data) //Write data for transfer
#define I2C_READ_DATA           	(I2C_array[module]->D)
#define I2C_GET_IRQ_FLAG        	(I2C_array[module]->S & I2C_S_IICIF_MASK)
#define I2C_CLEAR_IRQ_FLAG      	(I2C_array[module]->S |= I2C_S_IICIF_MASK)
#define I2C_GET_RX_ACK				(!(I2C_array[module]->S & I2C_S_RXAK_MASK))
#define I2C_SET_RX_MODE         	(I2C_array[module]->C1 &= ~I2C_C1_TX_MASK)
#define I2C_SET_TX_MODE         	(I2C_array[module]->C1 |= I2C_C1_TX_MASK)
#define I2C_SET_NACK	         	(I2C_array[module]->C1 |= I2C_C1_TXAK_MASK)
#define I2C_CLEAR_NACK      	 	(I2C_array[module]->C1 &= ~I2C_C1_TXAK_MASK)
#define I2C_CHECK_BUS		  		(!(I2C_array[module]->S & I2C_S_BUSY_MASK))
#define I2C_GET_TCF					(I2C_array[module]->S & I2C_S_TCF_MASK)
#define I2C_CLEAR_TCF            	(I2C_array[module]->S |= I2C_S_TCF_MASK)
#define I2C_RW_MASK					((0xFF <<1) + !I2C_Objects[module].mode)
#define I2C_ADDRESS_MASK			(((I2C_Objects[module].slave_address & 0x7F)<<1)+1)
#define I2C_ADRESS_RW_BYTE			(I2C_ADDRESS_MASK & I2C_RW_MASK)

// I2C_Object Macros
#define I2C_OBJ						(I2C_Objects[module])
#define I2C_OBJ_MODE				(I2C_Objects[module].mode)
#define I2C_OBJ_STATUS				(I2C_Objects[module].status)
#define I2C_OBJ_SLAVE_ADDR			(I2C_Objects[module].slave_address)
#define I2C_OBJ_RW_INDEX			(I2C_Objects[module].RW_index)
#define I2C_OBJ_R_BUFFER			(I2C_Objects[module].read_buffer)
#define I2C_OBJ_R_SIZE				(I2C_Objects[module].read_size)
#define I2C_OBJ_W_BUFFER			(I2C_Objects[module].write_buffer)
#define I2C_OBJ_W_BUFFER_D(x)		(I2C_Objects[module].write_buffer[(x)])
#define I2C_OBJ_W_SIZE				(I2C_Objects[module].write_size)
#define I2C_OBJ_SLAVE_REG			(I2C_Objects[module].slave_reg)

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH LOCAL SCOPE
 ******************************************************************************/

I2C_Type * I2C_array [] = I2C_BASE_PTRS;
I2C_Object_t I2C_Objects[I2C_M_Count];

static uint32_t ICR2SCLDivider[] = {
	20, 22, 24, 26, 28, 30, 34, 40, 28, 32, 36, 40, 44, 48, 56,
	68, 48, 56, 64, 72, 80, 88, 104, 128, 80, 96, 112, 128, 144,
	160, 192, 240, 160, 192, 224, 256, 288, 320, 384, 480, 320,
	384, 448, 512, 576, 640, 768, 960, 640, 768, 896, 1024, 1152,
	1280, 1536, 1920, 1280, 1536, 1792, 2048, 2304, 2560, 3072, 3840
};

// Declaring the baud rate configuration structure
typedef struct {
  uint8_t mul; 	// Multiplier Factor, register value
  uint8_t icr;	// ClockRate, register value
  uint32_t targetBaudRate;	// The target baud rate with the given register values
} BaudRate_t;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static BaudRate_t SetBaudRate (uint32_t desiredBaudRate);


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


void I2C_InitModule (I2C_Module_t module)
{

	I2C_Type * I2C = I2C_array[module];

	IRQn_Type I2C_IRQ_array[] = I2C_IRQS;
	IRQn_Type I2C_IRQ = I2C_IRQ_array[module];


	//Clock gating modulo I2C
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK | SIM_SCGC4_I2C1_MASK;
	SIM->SCGC1 |= SIM_SCGC1_I2C2_MASK;

	//Clock gating puertos
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK
			   | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;


	//Enable I2C control register del modulo y de sus interrupciones
	I2C->C1 = 0 | I2C_C1_IICEN_MASK | I2C_C1_IICIE_MASK;

	//Status register para accion inicial
	I2C->S = I2C_S_IICIF_MASK;


	BaudRate_t baudRate = SetBaudRate(20000);
	I2C->F = I2C_F_MULT(baudRate.mul) | I2C_F_ICR(baudRate.icr); //  set the I2C baud rate

	//Mux puertos en modo I2C
	if (module==0)
	{
	PORTE->PCR[I2C0_SDA_PIN]=0x0; //Clear all bits
	PORTE->PCR[I2C0_SDA_PIN]|=PORT_PCR_IRQC(PORT_eDisabled); //Disable interrupts
	PORTE->PCR[I2C0_SDA_PIN]|=PORT_PCR_MUX(PORT_mAlt5); 	 //Set MUX to I2C0
	PORTE->PCR[I2C0_SDA_PIN]|= PORT_PCR_ODE_MASK;

	PORTE->PCR[I2C0_SCL_PIN]=0x0; //Clear all bits
	PORTE->PCR[I2C0_SCL_PIN]|=PORT_PCR_IRQC(PORT_eDisabled); //Disable interrupts
	PORTE->PCR[I2C0_SCL_PIN]|=PORT_PCR_MUX(PORT_mAlt5); 	 //Set MUX to I2C0
	PORTE->PCR[I2C0_SCL_PIN]|= PORT_PCR_ODE_MASK;
	}


	//Configuracion de interrupcion
	NVIC_EnableIRQ(I2C_IRQ);

}

I2C_Status_t I2C_InitObject(I2C_Module_t module, I2C_Mode_t mode, uint8_t * read_buffer, size_t read_size,
		  uint8_t * write_buffer, size_t write_size, I2C_Address_t slave_address, I2C_Address_t slave_reg)
{

	// Set parameters for I2C_Object

	I2C_OBJ_MODE=mode;
	I2C_OBJ_SLAVE_ADDR=slave_address;
	I2C_OBJ_STATUS=I2C_Idle;
	I2C_OBJ_R_BUFFER=read_buffer;
	I2C_OBJ_R_SIZE=read_size;
	I2C_Objects[module].data=write_buffer[0];
	I2C_OBJ_W_BUFFER=write_buffer;
	I2C_OBJ_W_SIZE=write_size;
	I2C_OBJ_SLAVE_REG=slave_reg;

	// Initialize communication
	// Como esta parte es común a tanto escritura como lectura (a excepción del bit de R/W), se colocó aquíç

	if (I2C_CHECK_BUS)		//
	{
		I2C_CLEAR_NACK;
		I2C_SET_TX_MODE;	// Set to transmit mode
		I2C_START_SIGNAL;	// Start Signal (Setting Master Mode)
		I2C_WRITE_DATA(I2C_ADRESS_RW_BYTE);
		I2C_OBJ_STATUS=I2C_Address_RW;	//	Se pasa al estado I2C_Address_RW, esperando su acknowledge (en la interrupción)
		I2C_OBJ_RW_INDEX=0;
	}
	else
	{
		I2C_OBJ_STATUS=I2C_Fail;
	}
	return I2C_OBJ_STATUS;

}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/






// TODO: ver el tema de las salidas cuando hay error o termina, una función tipo finish_com o parecido si hay falta (creeria que no)
I2C_Status_t I2C_IRQHandler(I2C_Module_t module)
{
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
						if (I2C_OBJ_SLAVE_REG)
							I2C_WRITE_DATA(I2C_OBJ_SLAVE_REG);
						I2C_OBJ_STATUS=I2C_Write_Byte;	// Paso a escribir el primer byte
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
							I2C_WRITE_DATA(I2C_OBJ_W_BUFFER_D(I2C_OBJ_RW_INDEX));		// Debería ser I2C_WRITE_DATA(I2C_OBJ_W_BUFFER[I2C_OBJ_RW_INDEX])
							I2C_OBJ_RW_INDEX++;
						}
					}
					else	// No recibió el acknowledge
						I2C_OBJ_STATUS=I2C_Fail;
				}
				break;	// I2C_Write_Byte
				case I2C_Idle:
				case I2C_Read_Byte:
				case I2C_Read_Dummy:
				case I2C_Fail:
					break;
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
					uint8_t dummy = I2C_READ_DATA;
					if(I2C_OBJ_RW_INDEX >= I2C_OBJ_R_SIZE-1) // Si tengo que leer un único dato
						I2C_SET_NACK;
				}
				break;
			case I2C_Read_Dummy:
				I2C_OBJ_R_BUFFER[I2C_OBJ_RW_INDEX] = I2C_READ_DATA;
				if(I2C_OBJ_RW_INDEX >= I2C_OBJ_R_SIZE-1) // Si tengo que leer un único dato
				{
					I2C_STOP_SIGNAL;
					I2C_CLEAR_NACK;
					I2C_OBJ_RW_INDEX++;
					I2C_OBJ_STATUS=I2C_Idle;
				}
				else
				{
					I2C_OBJ_R_BUFFER[I2C_OBJ_RW_INDEX] = I2C_READ_DATA;
					I2C_OBJ_RW_INDEX++;
					I2C_OBJ_STATUS=I2C_Read_Byte;
				}
				break;
			case I2C_Read_Byte:
				if(I2C_OBJ_RW_INDEX == I2C_OBJ_R_SIZE-1) // Si estoy leyendo el último dato
				{
					I2C_SET_NACK;			// Desabilito el Transmit acknowledge para cortar la lectura
					I2C_CLEAR_NACK;			// Restablezco el Transmit acknowledge
					I2C_OBJ_R_BUFFER[I2C_OBJ_RW_INDEX] = I2C_READ_DATA;	//Leo el último dato
					I2C_OBJ_STATUS=I2C_Idle;
					I2C_STOP_SIGNAL;		// Emito la señal de stop
				}
				else	// Si no es el último, sigo leyendo
				{
					I2C_OBJ_R_BUFFER[I2C_OBJ_RW_INDEX] = I2C_READ_DATA;
					I2C_OBJ_RW_INDEX++;
				}
				break;
			case I2C_Fail:
			case I2C_Idle:
			case I2C_Write_Byte:
				break;
			}
			break; // I2C_Read
		}
	return I2C_OBJ_STATUS;
}

static BaudRate_t SetBaudRate(uint32_t desiredBaudRate)
{
  BaudRate_t setting = {.mul = 0, .icr = 0};
  uint32_t bestBaudRate = 0;
  uint32_t bestError = 0;
  uint32_t currentBaudRate = 0;
  uint32_t currentError = 0;

  for(uint8_t mul = 1 ; mul <= 4 ; mul = mul << 1)
  {
	for(uint8_t icr = 0 ; icr <= 0x3F ; icr++)
	{
	  currentBaudRate = I2C_BUS_CLOCK / (mul * ICR2SCLDivider[icr]);
	  currentError = currentBaudRate > desiredBaudRate ? currentBaudRate - desiredBaudRate : desiredBaudRate - currentBaudRate;
	  if (bestBaudRate == 0 || bestError > currentError)
	  {
		  bestBaudRate = currentBaudRate;
		  bestError = currentError;
		  setting.mul = mul;
		  setting.icr = icr;
		  setting.targetBaudRate = currentBaudRate;
	  }
	}
  }

  return setting;
}



__ISR__ I2C0_IRQHandler(void)
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
