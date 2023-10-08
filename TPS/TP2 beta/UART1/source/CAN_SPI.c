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



/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
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


	// 2- Reset mode
	uint8_t data[10];
	data[0] = 0b11000000;
	SPI_SendData(data, 1);
	while(SPI_Transmission_In_Process());



	// 3- Bit time configuration
	data[0] = WRITE_INSTRUCTION;
	data[1] = CNF1_ADDRESS;
	data[2] = 0b00000011; // SJW = 1TQ; BRP = 4 (3+1)
	while(SPI_Transmission_In_Process());
	SPI_SendData(data, 3);


	data[0] = WRITE_INSTRUCTION;
	data[1] = CNF2_ADDRESS;
	data[2] = 0b10110001; // btl=1; sam=0; phseg = 7 (6+1); prseg = 2 (1+1)
	while(SPI_Transmission_In_Process());
	SPI_SendData(data, 3);

	data[0] = WRITE_INSTRUCTION;
	data[1] = CNF3_ADDRESS;
	data[2] = 0b10000101; // sof = 1; WAKFL = 0; PHSEG2 = 6 (5+1)
	while(SPI_Transmission_In_Process());
	SPI_SendData(data, 3);


	// 4- Filter configurations
	data[0] = WRITE_INSTRUCTION;
	data[1] = RxM0SIDH;
	data[2] = 0b11111111;
	while(SPI_Transmission_In_Process());
	SPI_SendData(data, 3);

	data[0] = WRITE_INSTRUCTION;
	data[1] = RxM0SIDL;
	data[2] = 0b00000000;
	while(SPI_Transmission_In_Process());
	SPI_SendData(data, 3);

	data[0] = WRITE_INSTRUCTION;
	data[1] = RxF0SIDH;
	data[2] = 0b00100000;
	while(SPI_Transmission_In_Process());
	SPI_SendData(data, 3);

	data[0] = WRITE_INSTRUCTION;
	data[1] = RxF0SIDL;
	data[2] = 0b00000000;
	while(SPI_Transmission_In_Process());
	SPI_SendData(data, 3);

	// 5- Reception configurations
	data[0] = WRITE_INSTRUCTION;
	data[1] = RxB0CTRL;
	data[2] = 0b00000100;//Roll-over enabled (RX0 TO RX1)
	while(SPI_Transmission_In_Process());
	SPI_SendData(data, 3);

	data[0] = WRITE_INSTRUCTION;
	data[1] = RxB1CTRL;
	data[2] = 0b00000000;
	while(SPI_Transmission_In_Process());
	SPI_SendData(data, 3);


	// 6- Erase flags and enable interrupts
	data[0] = WRITE_INSTRUCTION;
	data[1] = CANINTE;
	data[2] = 0b00000011;
	while(SPI_Transmission_In_Process());
	SPI_SendData(data, 3);

	data[0] = WRITE_INSTRUCTION;
	data[1] = CANINTF;
	data[2] = 0b00000000;	// all flags in 0
	while(SPI_Transmission_In_Process());
	SPI_SendData(data, 3);

	// 7- Normal mode
	data[0] = BIT_MODIFY_INSTRUCTION;
	data[1] = CANCTRL;
	data[2] = 0b11000000; // Mask
	data[3] = 0b00000000;
	while(SPI_Transmission_In_Process());
	SPI_SendData(data, 4);


	queue_Init(0);
	queue_Init(1);

	// 3- Bit time configuration
		data[0] = READ_INSTRUCTION;
		data[1] = CNF1_ADDRESS;
		data[2] = 0b00000011; // SJW = 1TQ; BRP = 4 (3+1)
		while(SPI_Transmission_In_Process());
		SPI_SendData(data, 3);
		while(!SPI_Read_Status());

		uint8_t aux [4];
		SPI_Get_DataBytes(aux, 3);


		data[0] = READ_INSTRUCTION;
		data[1] = CNF2_ADDRESS;
		data[2] = 0b10110001; // btl=1; sam=0; phseg = 7 (6+1); prseg = 2 (1+1)
		SPI_SendData(data, 3);
		while(!SPI_Read_Status());

		data[0] = READ_INSTRUCTION;
		data[1] = CNF3_ADDRESS;
		data[2] = 0b10000101; // sof = 1; WAKFL = 0; PHSEG2 = 6 (5+1)
		SPI_SendData(data, 3);
		while(!SPI_Read_Status());


		// 4- Filter configurations
		data[0] = READ_INSTRUCTION;
		data[1] = RxM0SIDH;
		data[2] = 0b11111111;
		SPI_SendData(data, 3);
		while(!SPI_Read_Status());

		data[0] = READ_INSTRUCTION;
		data[1] = RxM0SIDL;
		data[2] = 0b00000000;
		SPI_SendData(data, 3);
		while(!SPI_Read_Status());

		data[0] = READ_INSTRUCTION;
		data[1] = RxF0SIDH;
		data[2] = 0b00100000;
		SPI_SendData(data, 3);
		while(!SPI_Read_Status());

		data[0] = READ_INSTRUCTION;
		data[1] = RxF0SIDL;
		data[2] = 0b00000000;
		SPI_SendData(data, 3);
		while(!SPI_Read_Status());

		// 5- Reception configurations
		data[0] = READ_INSTRUCTION;
		data[1] = RxB0CTRL;
		data[2] = 0b00000100;//Roll-over enabled (RX0 TO RX1)
		SPI_SendData(data, 3);
		while(!SPI_Read_Status());

		data[0] = READ_INSTRUCTION;
		data[1] = RxB1CTRL;
		data[2] = 0b00000000;
		SPI_SendData(data, 3);
		while(!SPI_Read_Status());


		// 6- Erase flags and enable interrupts
		data[0] = READ_INSTRUCTION;
		data[1] = CANINTE;
		data[2] = 0b00000011;
		SPI_SendData(data, 3);
		while(!SPI_Read_Status());

		data[0] = READ_INSTRUCTION;
		data[1] = CANINTF;
		data[2] = 0b00000000;	// all flags in 0
		SPI_SendData(data, 3);
		while(!SPI_Read_Status());

		// 7- Normal mode
		data[0] = READ_INSTRUCTION;
		data[1] = CANCTRL;
		data[2] = 0b11000000; // Mask
		SPI_SendData(data, 3);
		while(!SPI_Read_Status());

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

void CAN_SPI_SendByte(uint8_t byte)
{
	buffer_element_t event = {byte, 1};
	push_Queue_Element(1, event);
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
 * @brief Initializes the circular queue
 */
static void queue_Init (uint8_t id)
{
	can_spi_buffers[id].pin = can_spi_buffers[id].buffer;
	can_spi_buffers[id].pout = can_spi_buffers[id].pin;
	can_spi_buffers[id].num_Of_Words = 0;
}

/**
 * @brief Pushes an event to the queue
 * @param event The element to add to the queue
 * @return Number of pending events. Returns value OVERFLOW if the maximun number of events is reached
 */
static int8_t push_Queue_Element(uint8_t id, buffer_element_t event)
{
	// Check for EventQueue Overflow
	if (can_spi_buffers[id].num_Of_Words > BUFFER_SIZE-1)
	{
		return OVERFLOW;
	}

	*(can_spi_buffers[id].pin)++ = event;
	can_spi_buffers[id].num_Of_Words++;

	// Return pointer to the beginning if necessary
	if (can_spi_buffers[id].pin == BUFFER_SIZE + can_spi_buffers[id].buffer)
	{
		can_spi_buffers[id].pin = can_spi_buffers[id].buffer;
	}

	return can_spi_buffers[id].num_Of_Words;

}

/**
 * @brief Pulls the earliest event from the queue
 * @return Event_Type variable with the current event if no OVERFLOW is detected.
 */


static buffer_element_t pull_Queue_Element(uint8_t id)
{
	buffer_element_t event = *(can_spi_buffers[id].pout);

	if (can_spi_buffers[id].num_Of_Words == 0)
	{
		buffer_element_t event1 = {0,2};
		return event1;
	}

	can_spi_buffers[id].num_Of_Words--;
	can_spi_buffers[id].pout++;

	if (can_spi_buffers[id].pout == BUFFER_SIZE + can_spi_buffers[id].buffer)
	{
		can_spi_buffers[id].pout = can_spi_buffers[id].buffer;
	}

	return event;

}


/**
 * @brief Gets the status of the queue
 * @return Returns the number of pending events in the queue
 */
static uint8_t get_Queue_Status(uint8_t id)
{
	return can_spi_buffers[id].num_Of_Words;
}






/*******************************************************************************
 ******************************************************************************/
