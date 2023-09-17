
#include "hardware.h"
#include "UART.h"


#define UART_HAL_DEFAULT_BAUDRATE 9600

#define ISR_TDRE(x) (((x) & UART_S1_TDRE_MASK) != 0x0)
#define ISR_RDRF(x) (((x) & UART_S1_RDRF_MASK) != 0x0)


#define UART0_TX_PIN 	17   //PTB17
#define UART0_RX_PIN 	16   //PTB16


#define	BUFFER_SIZE	15
#define NONE_EV  0 //TODO
#define TOTAL_UARTS 5
#define OVERFLOW -1


typedef struct uart_queue{
	uint8_t *pin;
	uint8_t *pout;
	uint8_t queue[BUFFER_SIZE];
	uint8_t num_Of_Words;
}uart_queue_t;

uart_queue_t uart_queues[2*TOTAL_UARTS]; //doubles the value for input and output buffers

static unsigned char rx_flag=false;
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

void UART_Init (void)
{

// Note: 5.6 Clock Gating page 192
// Any bus access to a peripheral that has its clock disabled generates an error termination.


	    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
		SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;
		SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
		SIM->SCGC4 |= SIM_SCGC4_UART3_MASK;
		SIM->SCGC1 |= SIM_SCGC1_UART4_MASK;
		SIM->SCGC1 |= SIM_SCGC1_UART5_MASK;

		NVIC_EnableIRQ(UART0_RX_TX_IRQn);
		NVIC_EnableIRQ(UART1_RX_TX_IRQn);
		NVIC_EnableIRQ(UART2_RX_TX_IRQn);
		NVIC_EnableIRQ(UART3_RX_TX_IRQn);
		NVIC_EnableIRQ(UART4_RX_TX_IRQn);
		NVIC_EnableIRQ(UART5_RX_TX_IRQn);

		//UART0 Set UART Speed

		UART_SetBaudRate(UART0, UART_HAL_DEFAULT_BAUDRATE);

		//Configure UART0 TX and RX PINS

		PORTB->PCR[UART0_TX_PIN]=0x0; //Clear all bits
		PORTB->PCR[UART0_TX_PIN]|=PORT_PCR_MUX(PORT_mAlt3); 	 //Set MUX to UART0
		PORTB->PCR[UART0_TX_PIN]|=PORT_PCR_IRQC(PORT_eDisabled); //Disable interrupts
//----------------------------------------------------------------------------------
		PORTB->PCR[UART0_RX_PIN]=0x0; //Clear all bits
		PORTB->PCR[UART0_RX_PIN]|=PORT_PCR_MUX(PORT_mAlt3); 	 //Set MUX to UART0
		PORTB->PCR[UART0_RX_PIN]|=PORT_PCR_IRQC(PORT_eDisabled); //Disable interrupts


	//UART0 Baudrate Setup

		UART_SetBaudRate (UART0, 9600);

	//Enable UART0 Xmiter and Rcvr

		//UART0->C2=UART_C2_TE_MASK | UART_C2_RE_MASK;
		UART0->C2=UART_C2_TE_MASK | UART_C2_RE_MASK | UART_C2_RIE_MASK;




}


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


__ISR__ UART0_RX_TX_IRQHandler (UART_Type* uart_p, uint8_t id)
{
	unsigned char tmp;
	uint8_t tx_data;
	tmp = UART0->S1;// Dummy read to clear status register

	if(ISR_TDRE(tmp)) //checks if state is available
	{
		if (get_Queue_Status(id))
		{
			tx_data =  pull_Queue_Element(id);
			uart_p->D = tx_data;
		}

	}

	if(ISR_RDRF(tmp))
	{
		rx_flag=true;

		rx_data=UART0->D;
	}

}


unsigned char UART_Get_Status(void)
{
	return(rx_flag);
}


unsigned char UART_Get_Data(void)
{
	rx_flag=false;
	return(rx_data);
}



void UART_SendMsg(char* msg)
{
	uint32_t i = 0;
	while (msg[i]  != '\0')
	{
		UART_Send_Data(msg[i]);
		i++;
	}
	UART_Send_Data('\n');
	UART_Send_Data('\r');
}





/**
 * @brief Initializes the circular queue
 */
static void queue_Init (uint8_t id)
{
	uart_queues[id].pin=uart_queues[id].queue;
	uart_queues[id].pout=uart_queues[id].pin;
	uart_queues[id].num_Of_Words=0;

}

/**
 * @brief Pushes an event to the queue
 * @param event The element to add to the queue
 * @return Number of pending events. Returns value OVERFLOW if the maximun number of events is reached
 */
static int8_t push_Queue_Element(uint8_t id, uint8_t event)
{
	// Check for EventQueue Overflow
	if (uart_queues[id].num_Of_Words > BUFFER_SIZE-1)
	{
		return OVERFLOW;
	}

	*(uart_queues[id].pin)++ = event;
	uart_queues[id].num_Of_Words++;

	// Return pointer to the beginning if necessary
	if (uart_queues[id].pin == BUFFER_SIZE + uart_queues[id].queue)
	{
		uart_queues[id].pin = uart_queues[id].queue;
	}

	return uart_queues[id].num_Of_Words;

}

/**
 * @brief Pulls the earliest event from the queue
 * @return Event_Type variable with the current event if no OVERFLOW is detected.
 */
static uint8_t pull_Queue_Element(uint8_t id)
{
	uint8_t event = *(uart_queues[id].pout);

	if (uart_queues[id].num_Of_Words == 0)
	{
		return NONE_EV;
	}

	uart_queues[id].num_Of_Words--;
	uart_queues[id].pout++;

	if (uart_queues[id].pout == BUFFER_SIZE + uart_queues[id].queue)
	{
		uart_queues[id].pout = uart_queues[id].queue;
	}

	return event;

}


/**
 * @brief Gets the status of the queue
 * @return Returns the number of pending events in the queue
 */
static uint8_t get_Queue_Status(uint8_t id)
{
	return uart_queues[id].num_Of_Words;
}


