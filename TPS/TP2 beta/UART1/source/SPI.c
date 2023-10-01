/***************************************************************************//**
  @file     SPI.c

  @brief    SPI driver
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "hardware.h"
#include "SPI.h"



/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
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

void SPI_Init (void)
{

// Note: 5.6 Clock Gating page 192
// Any bus access to a peripheral that has its clock disabled generates an error termination.


	    SIM->SCGC6 |= SIM_SCGC6_SPI0_MASK;
	    SIM->SCGC6 |= SIM_SCGC6_SPI1_MASK;
	    SIM->SCGC3 |= SIM_SCGC3_SPI2_MASK ;

		NVIC_EnableIRQ(SPI0_IRQn);
		NVIC_EnableIRQ(SPI1_IRQn);
		NVIC_EnableIRQ(SPI2_IRQn);

		//Configure SPI0 PINS

		PORTD->PCR[SPI0_TX_PIN]=0x0; //Clear all bits
		PORTD->PCR[SPI0_TX_PIN]|=PORT_PCR_MUX(PORT_mAlt2); 	 //Set MUX to SPI0
		PORTD->PCR[SPI0_TX_PIN]|=PORT_PCR_IRQC(PORT_eDisabled); //Disable interrupts
//----------------------------------------------------------------------------------
		PORTD->PCR[SPI0_RX_PIN]=0x0; //Clear all bits
		PORTD->PCR[SPI0_RX_PIN]|=PORT_PCR_MUX(PORT_mAlt2); 	 //Set MUX to SPI0
		PORTD->PCR[SPI0_RX_PIN]|=PORT_PCR_IRQC(PORT_eDisabled); //Disable interrupts

//----------------------------------------------------------------------------------
		PORTD->PCR[SPI0_PCS_PIN]=0x0; //Clear all bits
		PORTD->PCR[SPI0_PCS_PIN]|=PORT_PCR_MUX(PORT_mAlt2); 	 //Set MUX to SPI0
		PORTD->PCR[SPI0_PCS_PIN]|=PORT_PCR_IRQC(PORT_eDisabled); //Disable interrupts
//----------------------------------------------------------------------------------
		PORTD->PCR[SPI0_SCK_PIN]=0x0; //Clear all bits
		PORTD->PCR[SPI0_SCK_PIN]|=PORT_PCR_MUX(PORT_mAlt2); 	 //Set MUX to SPI0
		PORTD->PCR[SPI0_SCK_PIN]|=PORT_PCR_IRQC(PORT_eDisabled); //Disable interrupts

	//Enable SPI0 Xmiter and Rcvr

		SPI0->MCR = 0x0;
		SPI0->MCR = SPI_MCR_MSTR_MASK | SPI_MCR_CONT_SCKE_MASK | SPI_MCR_HALT_MASK;
		SPI0->CTAR[0] = 0x0;
		SPI0->CTAR[0] = SPI_CTAR_FMSZ_MASK | SPI_CTAR_CPHA_MASK|SPI_CTAR_PBR_MASK|SPI_CTAR_BR(6);
		SPI0->SR = 0x0;
		SPI0->SR = SPI_SR_TXRXS_MASK ;
		SPI0->RSER = 0x0;
		SPI0->RSER = SPI_RSER_EOQF_RE_MASK | SPI_RSER_TFFF_RE_SHIFT | SPI_RSER_RFOF_RE_MASK;


		SPI0->MCR &= ~SPI_MCR_HALT_MASK;

		queue_Init(0);
		queue_Init(1);
}


/**
 * @brief
 * @param
 * @return
 */

uint8_t SPI_Get_Status(void)
{
	return(get_Queue_Status(0));
}

/**
 * @brief
 * @param
 * @return
 */

uint8_t SPI_Get_Data(void)
{
	return(pull_Queue_Element(0).data);
}

/**
 * @brief
 * @param
 * @return
 */

void SPI_SendByte(uint8_t byte)
{
	buffer_element_t event = {byte, 1};
	push_Queue_Element(1, event);
}

/**
 * @brief
 * @param
 * @return
 */

void SPI_SendMsg(uint8_t* msg)
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
		push_Queue_Element(1, event);
		i++;
	}

}




/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
/**
 * @brief Handles Spi interrupts
 */
__ISR__ SPI0_IRQHandler(void)
{
	uint8_t tmp;
	tmp = SPI0 -> SR;// Dummy read to clear status register

	// Receive FIFO Drain Flag
	if(SPI_SR_RFDF(tmp))
	{
		uint32_t rxdata = SPI0 -> POPR;
		buffer_element_t data_in= {rxdata, 0};
		push_Queue_Element(1, data_in); //positions itself in the receiver queue
		// check if flag should be cleared (Maybe better to do at the end of all interrupts
	}

	// End of Queue Frag
	if(SPI_SR_EOQF(tmp)) //checks if state is available
	{

	}

	// Transfer FIFO Fill Flag (1 if not empty)
	if(SPI_SR_TFFF(tmp))
	{
		if (get_Queue_Status(0))
		{
			buffer_element_t buffer_data_out = pull_Queue_Element(0);
			uint32_t data_out = SPI_PUSHR_CONT_MASK;
			if (buffer_data_out.end_of_data)
			{
				data_out |= SPI_PUSHR_EOQ_MASK;
			}
			data_out |= SPI_PUSHR_PCS(1);
			data_out |= buffer_data_out.data;
			SPI0 -> PUSHR = data_out;
		}
	}
}

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






