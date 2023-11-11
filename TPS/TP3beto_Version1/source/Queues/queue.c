	/***************************************************************************//**
  @file     queue.c
  @brief    Application functions
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


#include <Queues/queue.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/**
 * @brief Initializes the circular queue
 */
void queue_Init_uint8(queue_uint8_t* buffer)
{
	buffer->pin = buffer->queue;
	buffer->pout = buffer->pin;
	buffer->num_Of_Events = 0;
}

/**
 * @brief Pushes an event to the queue
 * @param event The element to add to the queue
 * @return Number of pending events. Returns value OVERFLOW if the maximun number of events is reached
 */
int8_t push_Queue_Element_uint8(queue_uint8_t* buffer, uint8_t event)
{
	// Check for EventQueue Overflow
	if (buffer->num_Of_Events > MAX_NUMBER_OF_ELEMENTS-1)
	{
		return OVERFLOW;
	}

	*(buffer->pin)++ = event;
	buffer->num_Of_Events++;

	// Return pointer to the beginning if necessary
	if (buffer->pin == MAX_NUMBER_OF_ELEMENTS + buffer->queue)
	{
		buffer->pin = buffer->queue;
	}

	return buffer->num_Of_Events;

}

/**
 * @brief Pulls the earliest event from the queue
 * @return Event_Type variable with the current event if no OVERFLOW is detected.
 */
uint8_t pull_Queue_Element_uint8(queue_uint8_t* buffer)
{
	uint8_t event = *(buffer->pout);

	if (buffer->num_Of_Events == 0)
	{
		return 0;
	}

	buffer->num_Of_Events--;
	buffer->pout++;

	if (buffer->pout == MAX_NUMBER_OF_ELEMENTS + buffer->queue)
	{
		buffer->pout = buffer->queue;
	}

	return event;

}


/**
 * @brief Gets the status of the queue
 * @return Returns the number of pending events in the queue
 */
uint8_t get_Queue_Status_uint8(queue_uint8_t* buffer)
{
	return buffer->num_Of_Events;
}

////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Initializes the circular queue
 */
void queue_Init_uint16(queue_uint16_t* buffer)
{
	buffer->pin = buffer->queue;
	buffer->pout = buffer->pin;
	buffer->num_Of_Events = 0;
}

/**
 * @brief Pushes an event to the queue
 * @param event The element to add to the queue
 * @return Number of pending events. Returns value OVERFLOW if the maximun number of events is reached
 */
int8_t push_Queue_Element_uint16(queue_uint16_t* buffer, uint16_t event)
{
	// Check for EventQueue Overflow
	if (buffer->num_Of_Events > MAX_NUMBER_OF_ELEMENTS-1)
	{
		return OVERFLOW;
	}

	*(buffer->pin)++ = event;
	buffer->num_Of_Events++;

	// Return pointer to the beginning if necessary
	if (buffer->pin == MAX_NUMBER_OF_ELEMENTS + buffer->queue)
	{
		buffer->pin = buffer->queue;
	}

	return buffer->num_Of_Events;

}

/**
 * @brief Pulls the earliest event from the queue
 * @return Event_Type variable with the current event if no OVERFLOW is detected.
 */
uint16_t pull_Queue_Element_uint16(queue_uint16_t* buffer)
{
	uint16_t event = *(buffer->pout);

	if (buffer->num_Of_Events == 0)
	{
		return 0;
	}

	buffer->num_Of_Events--;
	buffer->pout++;

	if (buffer->pout == MAX_NUMBER_OF_ELEMENTS + buffer->queue)
	{
		buffer->pout = buffer->queue;
	}

	return event;

}


/**
 * @brief Gets the status of the queue
 * @return Returns the number of pending events in the queue
 */
uint8_t get_Queue_Status_uint16(queue_uint16_t* buffer)
{
	return buffer->num_Of_Events;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////7
/**
 * @brief Initializes the circular queue
 */
void queue_Init_uint32(queue_uint32_t* buffer)
{
	buffer->pin = buffer->queue;
	buffer->pout = buffer->pin;
	buffer->num_Of_Events = 0;
}

/**
 * @brief Pushes an event to the queue
 * @param event The element to add to the queue
 * @return Number of pending events. Returns value OVERFLOW if the maximun number of events is reached
 */
int8_t push_Queue_Element_uint32(queue_uint32_t* buffer, uint32_t event)
{
	// Check for EventQueue Overflow
	if (buffer->num_Of_Events > MAX_NUMBER_OF_ELEMENTS-1)
	{
		return OVERFLOW;
	}

	*(buffer->pin)++ = event;
	buffer->num_Of_Events++;

	// Return pointer to the beginning if necessary
	if (buffer->pin == MAX_NUMBER_OF_ELEMENTS + buffer->queue)
	{
		buffer->pin = buffer->queue;
	}

	return buffer->num_Of_Events;

}

/**
 * @brief Pulls the earliest event from the queue
 * @return Event_Type variable with the current event if no OVERFLOW is detected.
 */
uint32_t pull_Queue_Element_uint32(queue_uint32_t* buffer)
{
	uint32_t event = *(buffer->pout);

	if (buffer->num_Of_Events == 0)
	{
		return 0;
	}

	buffer->num_Of_Events--;
	buffer->pout++;

	if (buffer->pout == MAX_NUMBER_OF_ELEMENTS + buffer->queue)
	{
		buffer->pout = buffer->queue;
	}

	return event;

}


/**
 * @brief Gets the status of the queue
 * @return Returns the number of pending events in the queue
 */
uint8_t get_Queue_Status_uint32(queue_uint32_t* buffer)
{
	return buffer->num_Of_Events;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/




/*******************************************************************************
 ******************************************************************************/
