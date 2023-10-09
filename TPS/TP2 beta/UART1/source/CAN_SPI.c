/***************************************************************************//**
  @file     CAN_SPI.c

  @brief    CAN_sPI driver
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "hardware.h"
#include "CAN_SPI.h"
#include "SPI.h"
#include "gpio.h"



/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define INT_PIN      16
#define INT_PORT     PC

#define	WRITE_INSTRUCTION 		0b00000010
#define	BIT_MODIFY_INSTRUCTION 	0b00000101
#define	READ_INSTRUCTION 		0b00000011

#define	CNF1_ADDRESS	0b00101010
#define	CNF2_ADDRESS	0b00101001
#define	CNF3_ADDRESS	0b00101000

#define RxM0SIDH		0b00100000
#define RxM0SIDL		0b00100001

#define RxF0SIDH		0b00000000
#define RxF0SIDL		0b00000001

#define	RxB0CTRL		0b01100000
#define RxB1CTRL		0b01110000

#define	CANINTE			0b00101011
#define	CANINTF			0b00101100

#define	CANCTRL			0b00001111

#define	TXB0CTRL		0b00110000
#define	TXB1CTRL		0b01000000
#define	TXB2CTRL		0b01010000

#define	TXB0SIDH		0b00110001
#define	TXB1SIDH		0b01000001
#define	TXB2SIDH		0b01010001

#define	TXB0SIDL		0b00110010
#define	TXB1SIDL		0b01000010
#define	TXB2SIDL		0b01010010

#define	TXB0DLC			0b00110101
#define	TXB1DLC			0b01000101
#define	TXB2DLC			0b01010101

#define	BUFFER_SIZE	 20
#define OVERFLOW     -1

typedef struct buffer_element{
	uint8_t data;
	uint8_t end_of_data; //last byte if value is 1
}buffer_element_t;

typedef struct spi_buffer{
	buffer_element_t *pin;
	buffer_element_t *pout;
	buffer_element_t buffer[BUFFER_SIZE];
	uint8_t num_Of_Words;
}spi_buffer_t;

spi_buffer_t can_spi_buffers[2]; //doubles the value for input and output buffers. 1 is input, 0 is output.

typedef enum
{
	PORT_mAnalog,
	PORT_mGPIO,
	PORT_mAlt2,
	PORT_mAlt3,
	PORT_mAlt4,
	PORT_mAlt5,
	PORT_mAlt6,
	PORT_mAlt7,

} PORTMux_t;

typedef enum
{
	PORT_eDisabled				= 0x00,
	PORT_eDMARising				= 0x01,
	PORT_eDMAFalling			= 0x02,
	PORT_eDMAEither				= 0x03,
	PORT_eInterruptDisasserted	= 0x08,
	PORT_eInterruptRising		= 0x09,
	PORT_eInterruptFalling		= 0x0A,
	PORT_eInterruptEither		= 0x0B,
	PORT_eInterruptAsserted		= 0x0C,
} PORTEvent_t;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
*******************************************************************************/
/**
 * @brief Initializes the circular queue
 */
static void queue_Init (uint8_t id);


/**
 * @brief Pushes an event to the queue
 * @param event The element to add to the queue
 * @return Number of pending events. Returns value OVERFLOW if the maximun number of events is reached
 */
static int8_t push_Queue_Element(uint8_t id, buffer_element_t event);


/**
 * @brief Pulls the earliest event from the queue
 * @return Event_Type variable with the current event if no OVERFLOW is detected.
 */
static buffer_element_t pull_Queue_Element(uint8_t id);


/**
 * @brief Gets the status of the queue
 * @return Returns the number of pending events in the queue
 */
static uint8_t get_Queue_Status(uint8_t id);


/**
 * @brief
 * @return
 */
static uint8_t load_TX_buffer(uint8_t abc, uint8_t * bytes, uint8_t num_bytes, void cb(void));

/**
 * @brief
 * @return
 */
static uint8_t write_SPICAN(uint8_t address, uint8_t * bytes_data, uint8_t num_bytes_data, void cb(void));

/**
 * @brief
 * @return
 */
static uint8_t read_SPICAN(uint8_t address, uint8_t num_bytes_to_read, void cb(void));


/**
 * @brief
 * @return
 */
static uint8_t read_RX_buffer(uint8_t nm, uint8_t num_bytes_to_read, void cb(void));

/**
 * @brief
 * @return
 */
static uint8_t read_status(void cb(void));


/**
 * @brief
 * @return
 */
static RTS(uint8_t txn, void cb(void));

static parseData(RXB_RAWDATA_t * rawdata, uint8_t *data_to_send);



/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



/**
 * @brief
 * @param
 * @return
 */

void CAN_SPI_Init (void)
{
	// 1- SPI in mode 0,0
	SPI_Init();

	// Set interrupt pin for RX interrupt detection
	gpioMode (PORTNUM2PIN(INT_PIN, INT_PORT), INPUT_PULLDOWN);
	gpioIRQ_Config (PORTNUM2PIN(INT_PIN, INT_PORT), PORT_eInterruptFalling);


	// 2- Reset mode
	uint8_t data[10];
	data[0] = 0b11000000;
	SPI_SendData(data, 1, 0);
	while(SPI_Transmission_In_Process());


	// 3- Bit time configuration
	data[0] = WRITE_INSTRUCTION;
	data[1] = CNF1_ADDRESS;
	data[2] = 0b00000011; // SJW = 1TQ; BRP = 4 (3+1)
	SPI_SendData(data, 3, 0);
	while(SPI_Transmission_In_Process());


	data[0] = WRITE_INSTRUCTION;
	data[1] = CNF2_ADDRESS;
	data[2] = 0b10110001; // btl=1; sam=0; phseg = 7 (6+1); prseg = 2 (1+1)
	SPI_SendData(data, 3, 0);
	while(SPI_Transmission_In_Process());

	data[0] = WRITE_INSTRUCTION;
	data[1] = CNF3_ADDRESS;
	data[2] = 0b10000101; // sof = 1; WAKFL = 0; PHSEG2 = 6 (5+1)
	while(SPI_Transmission_In_Process());
	SPI_SendData(data, 3, 0);


	// 4- Filter configurations
	data[0] = WRITE_INSTRUCTION;
	data[1] = RxM0SIDH;
	data[2] = 0b11111111;
	while(SPI_Transmission_In_Process());
	SPI_SendData(data, 3, 0);

	data[0] = WRITE_INSTRUCTION;
	data[1] = RxM0SIDL;
	data[2] = 0b00000000;
	while(SPI_Transmission_In_Process());
	SPI_SendData(data, 3, 0);

	data[0] = WRITE_INSTRUCTION;
	data[1] = RxF0SIDH;
	data[2] = 0b00100000;
	while(SPI_Transmission_In_Process());
	SPI_SendData(data, 3, 0);

	data[0] = WRITE_INSTRUCTION;
	data[1] = RxF0SIDL;
	data[2] = 0b00000000;
	while(SPI_Transmission_In_Process());
	SPI_SendData(data, 3, 0);

	// 5- Reception configurations
	data[0] = WRITE_INSTRUCTION;
	data[1] = RxB0CTRL;
	data[2] = 0b00000100;//Roll-over enabled (RX0 TO RX1)
	while(SPI_Transmission_In_Process());
	SPI_SendData(data, 3, 0);

	data[0] = WRITE_INSTRUCTION;
	data[1] = RxB1CTRL;
	data[2] = 0b00000000;
	while(SPI_Transmission_In_Process());
	SPI_SendData(data, 3, 0);


	// 6- Erase flags and enable interrupts
	data[0] = WRITE_INSTRUCTION;
	data[1] = CANINTE;
	data[2] = 0b00000011;
	while(SPI_Transmission_In_Process());
	SPI_SendData(data, 3, 0);

	data[0] = WRITE_INSTRUCTION;
	data[1] = CANINTF;
	data[2] = 0b00000000;	// all flags in 0
	while(SPI_Transmission_In_Process());
	SPI_SendData(data, 3, 0);

	// 7- Normal mode
	data[0] = BIT_MODIFY_INSTRUCTION;
	data[1] = CANCTRL;
	data[2] = 0b11000000; // Mask
	data[3] = 0b00000000;
	while(SPI_Transmission_In_Process());
	SPI_SendData(data, 4, 0);

}


/**
 * @brief
 * @param
 * @return
 */

uint8_t CAN_SPI_Get_Status(void)
{
	return(get_Queue_Status(0));
}

/**
 * @brief
 * @param
 * @return
 */

uint8_t CAN_SPI_Get_Data(void)
{
	return(pull_Queue_Element(0).data);
}


/**
 * @brief
 * @param
 * @return
 */

void CAN_SPI_ReceiveInfo(void)
{
	switch(sendState)
	{
	case 0:
		read_SPICAN(CANINTF, uint8_t num_bytes_to_read, void cb(void))
		parseData(rawdata, data_to_send);
		RX_STATUS_buffer(&CAN_SPI_SendInfo);
		sendState = 1;
		break;

	case 1:
		SPI_Get_DataBytes(aux, 3);
		uint8_t rxStatus = aux[1];

		if(rxStatus & 0b00000100)	// if TX
		{
			load_TX_buffer(0b000000000, data_to_send, 13, &CAN_SPI_SendInfo);
			number_buffer = 0;
			sendState = 2;
		}
		else if(rxStatus & 0b00010000)
		{
			load_TX_buffer(0b000000010, data_to_send, 13, &CAN_SPI_SendInfo);
			number_buffer = 1;
			sendState = 2;
		}
		else if(rxStatus & 0b01000000)
		{
			load_TX_buffer(0b000000100, data_to_send, 13, &CAN_SPI_SendInfo);
			number_buffer = 2;
			sendState = 2;
		}
		else
		{
			sendState = 0;
		}

		break;

	case 2:
		RTS(number_buffer, &CAN_SPI_SendInfo);
		sendState = 3;

	case 3:
		// Can data started to be sent
		sendState = 0;
		break;

	default:
		sendState = 0;
		break;


		}
	}

}




/**
 * @brief
 * @param
 * @return
 */

void CAN_SPI_SendInfo(RXB_RAWDATA_t * rawdata)
{
	// Initial Send Info state
	static uint8_t sendState = 0;
	static uint8_t aux[16];
	static uint8_t data_to_send[16];
	static uint8_t number_buffer = 0;

	switch(sendState)
	{
	case 0:
		parseData(rawdata, data_to_send);
		RX_STATUS_buffer(&CAN_SPI_SendInfo);
		sendState = 1;
		break;

	case 1:
		SPI_Get_DataBytes(aux, 3);
		uint8_t rxStatus = aux[1];

		if(rxStatus & 0b00000100)	// if TX
		{
			load_TX_buffer(0b000000000, data_to_send, 13, &CAN_SPI_SendInfo);
			number_buffer = 0;
			sendState = 2;
		}
		else if(rxStatus & 0b00010000)
		{
			load_TX_buffer(0b000000010, data_to_send, 13, &CAN_SPI_SendInfo);
			number_buffer = 1;
			sendState = 2;
		}
		else if(rxStatus & 0b01000000)
		{
			load_TX_buffer(0b000000100, data_to_send, 13, &CAN_SPI_SendInfo);
			number_buffer = 2;
			sendState = 2;
		}
		else
		{
			sendState = 0;
		}

		break;

	case 2:
		RTS(number_buffer, &CAN_SPI_SendInfo);
		sendState = 3;

	case 3:
		// Can data started to be sent
		sendState = 0;
		break;

	default:
		sendState = 0;
		break;


	}
}

/**
 * @brief
 * @param
 * @return
 */

void CAN_SPI_SendMsg(uint8_t* msg)
{
	uint32_t i = 0;
	while (msg[i]  != '\0')
	{
		uint8_t end_of_data = 0;
		if(msg[i+1] == '\0')
		{
			end_of_data = 1;
		}
		buffer_element_t event = {msg[i], end_of_data};
		push_Queue_Element(0, event);
		i++;
	}

	// Enable TFFF flag to start pushing data to the queue
	SPI0->RSER |= SPI_RSER_TFFF_RE_MASK;

	// Enable transfer
	//SPI0 -> SR |= SPI_SR_EOQF_MASK;

}




/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/**
 * @brief Gets the status of the queue
 * @return Returns the number of pending events in the queue
 */
static uint8_t get_Queue_Status(uint8_t id)
{
	return can_spi_buffers[id].num_Of_Words;
}


/**
 * @brief
 * @return
 */
static uint8_t load_TX_buffer(uint8_t abc, uint8_t * bytes_data, uint8_t num_bytes_data, void cb(void))
{
	uint8_t aux[16];
	aux[0] = 0b01000000 | abc;
	uint32_t i;
	for(i = 0; i < num_bytes_data; i++)
	{
		aux[i+1] = bytes_data[i];
	}
	SPI_SendData(aux, num_bytes_data + 1, cb);
}

/**
 * @brief
 * @return
 */
static uint8_t write_SPICAN(uint8_t address, uint8_t * bytes_data, uint8_t num_bytes_data, void cb(void))
{
	uint8_t aux[16];
	aux[0] = WRITE_INSTRUCTION;
	aux[1] = address;
	uint32_t i;
	for(i = 0; i < num_bytes_data; i++)
	{
		aux[i+2] = bytes_data[i];
	}
	SPI_SendData(aux, num_bytes_data + 2, cb);
}

/**
 * @brief
 * @return
 */
static uint8_t read_SPICAN(uint8_t address, uint8_t num_bytes_to_read, void cb(void))
{
	uint8_t aux[16];
	aux[0] = READ_INSTRUCTION;
	aux[1] = address;
	uint32_t i;
	for(i = 0; i < num_bytes_to_read; i++)
	{
		aux[i+2] = 0;
	}
	SPI_SendData(aux, num_bytes_to_read + 2, cb);
}

/**
 * @brief
 * @return
 */
static uint8_t read_RX_buffer(uint8_t nm, uint8_t num_bytes_to_read, void cb(void))
{
	uint8_t aux[16];
	aux[0] = 0b10010000 | nm;
	uint32_t i;
	for(i = 0; i < num_bytes_to_read; i++)
	{
		aux[i+1] = 0;
	}
	SPI_SendData(aux, num_bytes_to_read + 1, cb);
}

/**
 * @brief
 * @return
 */
static uint8_t read_status(void cb(void))
{
	uint8_t aux[16];
	aux[0] = 0b1010000;
	uint32_t i;
	for(i = 0; i < 2; i++)
	{
		aux[i+1] = 0;
	}
	SPI_SendData(aux, 3, cb);
}

/**
 * @brief
 * @return
 */
static RTS(uint8_t txn, void cb(void))
{
	uint8_t aux[16];
	aux[0] = 0b10000000 | (1 << txn);
	SPI_SendData(aux, 1, cb);
}



/**
 * @brief
 * @return
 */
static parseData(RXB_RAWDATA_t * rawdata, uint8_t *data_to_send)
{
	data_to_send[0] = (rawdata->SID >> 3);
	data_to_send[1] = (rawdata->SID << 5) & 0b11100000;
	data_to_send[2] = 0; // EID not used
	data_to_send[3] = 0;
	data_to_send[4] = rawdata->DLC;
	uint32_t i = 0;
	for(i = 0; i < 8; i++)
	{
		data_to_send[i+5] = rawdata->Dn[i];
	}
}


__ISR__ PORTD_IRQHandler(void)
{
	PORTC->PCR[INT_PIN] |= PORT_PCR_ISF_MASK;
	CAN_SPI_ReceiveInfo();

}




/*******************************************************************************
 ******************************************************************************/
