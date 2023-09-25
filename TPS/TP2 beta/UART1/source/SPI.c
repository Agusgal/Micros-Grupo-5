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
#define UART_HAL_DEFAULT_BAUDRATE 9600

#define ISR_TDRE(x) (((x) & UART_S1_TDRE_MASK) != 0x0)
#define ISR_RDRF(x) (((x) & UART_S1_RDRF_MASK) != 0x0)


#define SPI0_PCS_PIN	0 //PTD0
#define SPI0_SCK_PIN	1 //PTD1
#define SPI0_TX_PIN 	2 //PTD2
#define SPI0_RX_PIN 	3 //PTD3

#define	BUFFER_SIZE	 20
#define NONE_EV      24
#define TOTAL_UARTS   6
#define OVERFLOW     -1


typedef struct uart_buffer{
	uint8_t *pin;
	uint8_t *pout;
	uint8_t buffer[BUFFER_SIZE];
	uint8_t num_Of_Words;
}uart_buffer_t;

uart_buffer_t uart_buffers[2 * TOTAL_UARTS]; //doubles the value for input and output buffers

static unsigned char rx_flag = false;
static unsigned char rx_data;



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
static int8_t push_Queue_Element(uint8_t id, uint8_t event);


/**
 * @brief Pulls the earliest event from the queue
 * @return Event_Type variable with the current event if no OVERFLOW is detected.
 */
static uint8_t pull_Queue_Element(uint8_t id);


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

		//UART0 Set UART Speed

	//	UART_SetBaudRate(UART0, UART_HAL_DEFAULT_BAUDRATE);

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
	//UART0 Baudrate Setup

		//UART_SetBaudRate (UART0, 9600);
		//UART_SetBaudRate (UART1, 9600);

	//Enable UART0 Xmiter and Rcvr

		//UART0->C2=UART_C2_TE_MASK | UART_C2_RE_MASK;
		//UART0->C2=UART_C2_TE_MASK| UART_C2_RE_MASK | UART_C2_RIE_MASK;

		SPI0->MCR = 0x0;
		SPI0->MCR = SPI_MCR_MSTR_MASK | SPI_MCR_CONT_SCKE_MASK | SPI_MCR_HALT_MASK;
		SPI0->CTAR = 0x0;
		SPI0->CTAR = SPI_CTAR_FMSZ_MASK | SPI_CTAR_CPHA_MASK | ; //TODO: SETEAR EL BAUDRATE






		SPI0->MCR &= ~SPI_MCR_HALT_MASK;
		for (int i = 0; i<12 ; i++) //initializes transmitter and receiver queues for all uarts
		{
			queue_Init(i);
		}




}

/**
 * @brief
 * @param
 * @return
 */

void UART_SetBaudRate (UART_Type *uart, uint32_t baudrate)
{
	uint16_t sbr, brfa;
	uint32_t clock;

	clock = ((uart == UART0) || (uart == UART1))?(__CORE_CLOCK__):(__CORE_CLOCK__ >> 1);

	baudrate = ((baudrate == 0)?(UART_HAL_DEFAULT_BAUDRATE):
			((baudrate > 0x1FFF)?(UART_HAL_DEFAULT_BAUDRATE):(baudrate)));

	sbr = clock / (baudrate << 4);               // sbr = clock/(Baudrate x 16)
	brfa = (clock << 1) / baudrate - (sbr << 5); // brfa = 2*Clock/baudrate - 32*sbr

	uart->BDH = UART_BDH_SBR(sbr >> 8);
	uart->BDL = UART_BDL_SBR(sbr);
	uart->C4 = (uart->C4 & ~UART_C4_BRFA_MASK) | UART_C4_BRFA(brfa);
}

/**
 * @brief
 * @param
 * @return
 */

void UART_rx_tx_irq_handler (UART_Type* uart_p, uint8_t id)
{
	unsigned char tmp;
	uint8_t tx_data;
	tmp = uart_p -> S1;// Dummy read to clear status register

	if(ISR_TDRE(tmp)) //checks if state is available
	{
		if (get_Queue_Status(id))
		{
			tx_data =  pull_Queue_Element(id);
			uart_p->D = tx_data;
		}
		else
		{
			UART0->C2 &= ~UART_C2_TIE_MASK;
		}

	}

	if(ISR_RDRF(tmp))
	{
		push_Queue_Element(id + TOTAL_UARTS, uart_p ->D); //positions itself in the receiver queue
	}

}

/**
 * @brief
 * @param
 * @return
 */

unsigned char UART_Get_Status(uint8_t id)
{
	return(get_Queue_Status(id + TOTAL_UARTS));
}

/**
 * @brief
 * @param
 * @return
 */

unsigned char UART_Get_Data(uint8_t id)
{
	return(pull_Queue_Element(id + TOTAL_UARTS));
}

/**
 * @brief
 * @param
 * @return
 */

void UART_SendChar(char msg, uint8_t id)
{
		push_Queue_Element(id, msg);

		UART0->C2 |= UART_C2_TIE_MASK;
}

/**
 * @brief
 * @param
 * @return
 */

void UART_SendMsg(char* msg, uint8_t id)
{
	uint32_t i = 0;
	while (msg[i]  != '\0')
	{
		push_Queue_Element(id, msg[i]);
		i++;
	}

	push_Queue_Element(id,  '\n');
	push_Queue_Element(id,  '\r');
	//UART_Send_Data('\n');
	//UART_Send_Data('\r');

	UART0->C2 |= UART_C2_TIE_MASK;
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
	uart_buffers[id].pin = uart_buffers[id].buffer;
	uart_buffers[id].pout = uart_buffers[id].pin;
	uart_buffers[id].num_Of_Words = 0;
}

/**
 * @brief Pushes an event to the queue
 * @param event The element to add to the queue
 * @return Number of pending events. Returns value OVERFLOW if the maximun number of events is reached
 */
static int8_t push_Queue_Element(uint8_t id, uint8_t event)
{
	// Check for EventQueue Overflow
	if (uart_buffers[id].num_Of_Words > BUFFER_SIZE-1)
	{
		return OVERFLOW;
	}

	*(uart_buffers[id].pin)++ = event;
	uart_buffers[id].num_Of_Words++;

	// Return pointer to the beginning if necessary
	if (uart_buffers[id].pin == BUFFER_SIZE + uart_buffers[id].buffer)
	{
		uart_buffers[id].pin = uart_buffers[id].buffer;
	}

	return uart_buffers[id].num_Of_Words;

}

/**
 * @brief Pulls the earliest event from the queue
 * @return Event_Type variable with the current event if no OVERFLOW is detected.
 */
static uint8_t pull_Queue_Element(uint8_t id)
{
	uint8_t event = *(uart_buffers[id].pout);

	if (uart_buffers[id].num_Of_Words == 0)
	{
		return NONE_EV;
	}

	uart_buffers[id].num_Of_Words--;
	uart_buffers[id].pout++;

	if (uart_buffers[id].pout == BUFFER_SIZE + uart_buffers[id].buffer)
	{
		uart_buffers[id].pout = uart_buffers[id].buffer;
	}

	return event;

}


/**
 * @brief Gets the status of the queue
 * @return Returns the number of pending events in the queue
 */
static uint8_t get_Queue_Status(uint8_t id)
{
	return uart_buffers[id].num_Of_Words;
}



__ISR__ UART0_RX_TX_IRQHandler (void)
{
    UART_rx_tx_irq_handler(UART0, 0);
}

__ISR__ UART1_RX_TX_IRQHandler (void)
{
    UART_rx_tx_irq_handler(UART1, 1);
}

__ISR__ UART2_RX_TX_IRQHandler (void)
{
    UART_rx_tx_irq_handler(UART2, 2);
}

__ISR__ UART3_RX_TX_IRQHandler (void)
{
    UART_rx_tx_irq_handler(UART3, 3);
}

__ISR__ UART4_RX_TX_IRQHandler (void)
{
    UART_rx_tx_irq_handler(UART4, 4);
}





/*******************************************************************************
 ******************************************************************************/






