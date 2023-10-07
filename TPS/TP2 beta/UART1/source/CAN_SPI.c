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
#define IS_RFDF(tmp)	((tmp) & SPI_SR_RFDF_MASK)
#define IS_TFFF(tmp)	((tmp) & SPI_SR_TFFF_MASK)
#define IS_EOQF(tmp)	((tmp) & SPI_SR_EOQF_MASK)

#define SPI0_PCS_PIN	0 //PTD0
#define SPI0_SCK_PIN	1 //PTD1
#define SPI0_TX_PIN 	2 //PTD2
#define SPI0_RX_PIN 	3 //PTD3

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

spi_buffer_t spi_buffers[2]; //doubles the value for input and output buffers. 1 is input, 0 is output.

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
	uint8_t data = 0b11000000;
	SPI_SendData(&data, 1);







	queue_Init(0);
	queue_Init(1);
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
	spi_buffers[id].pin = spi_buffers[id].buffer;
	spi_buffers[id].pout = spi_buffers[id].pin;
	spi_buffers[id].num_Of_Words = 0;
}

/**
 * @brief Pushes an event to the queue
 * @param event The element to add to the queue
 * @return Number of pending events. Returns value OVERFLOW if the maximun number of events is reached
 */
static int8_t push_Queue_Element(uint8_t id, buffer_element_t event)
{
	// Check for EventQueue Overflow
	if (spi_buffers[id].num_Of_Words > BUFFER_SIZE-1)
	{
		return OVERFLOW;
	}

	*(spi_buffers[id].pin)++ = event;
	spi_buffers[id].num_Of_Words++;

	// Return pointer to the beginning if necessary
	if (spi_buffers[id].pin == BUFFER_SIZE + spi_buffers[id].buffer)
	{
		spi_buffers[id].pin = spi_buffers[id].buffer;
	}

	return spi_buffers[id].num_Of_Words;

}

/**
 * @brief Pulls the earliest event from the queue
 * @return Event_Type variable with the current event if no OVERFLOW is detected.
 */


static buffer_element_t pull_Queue_Element(uint8_t id)
{
	buffer_element_t event = *(spi_buffers[id].pout);

	if (spi_buffers[id].num_Of_Words == 0)
	{
		buffer_element_t event1 = {0,2};
		return event1;
	}

	spi_buffers[id].num_Of_Words--;
	spi_buffers[id].pout++;

	if (spi_buffers[id].pout == BUFFER_SIZE + spi_buffers[id].buffer)
	{
		spi_buffers[id].pout = spi_buffers[id].buffer;
	}

	return event;

}


/**
 * @brief Gets the status of the queue
 * @return Returns the number of pending events in the queue
 */
static uint8_t get_Queue_Status(uint8_t id)
{
	return spi_buffers[id].num_Of_Words;
}






/*******************************************************************************
 ******************************************************************************/
