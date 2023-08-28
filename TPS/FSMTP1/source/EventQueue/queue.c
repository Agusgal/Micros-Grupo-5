/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Bruno Di Sanzo
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "queue.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define	MAX_NUMBER_OF_EVENTS

static Event_Type queue [MAX_NUMBER_OF_EVENTS];
static uint8_t num_Of_Events = 0;
static uint8_t *pin, *pout;
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
void queue_Init (void)
{
	pin = queue;
	pout = pin;
	num_Of_Events = 0;
}

/**
 * @brief Pushes an event to the queue
 * @param event The element to add to the queue
 * @return Number of pending events. Returns value OVERFLOW if the maximun number of events is reached
 */
uint8_t push_Queue_Element(Event_Type event)
{
	// Check for EventQueue Overflow
	if (num_Of_Events > MAX_NUMBER_OF_EVENTS-1)
	{
		return OVERFLOW;
	}

	*pin++ = event;
	num_Of_Events++;

	// Return pointer to the beginning if necessary
	if (pin == MAX_NUMBER_OF_EVENTS + queue)
	{
		pin = queue;
	}

	return num_Of_Events;

}

/**
 * @brief Pulls the earliest event from the queue
 * @param queueStatus Pointer to a variable that stores the status of the queue. If no events, OVERFLOW is returned.
 * @return Event_Type variable with the current event if no OVERFLOW is detected.
 */
Event_Type pull_Queue_Element(uint8_t *queueStatus)
{
	EventType event = {0,0,0};
	if (number_Of_Elements < 1)
	{
		*queueStatus = OVERFLOW;
		return event;
	}
	event = *pout++;
	*queueStatus = --number_Of_Elements;

	if (pout == MAX_NUMBER_OF_EVENTS + queue)
	{
		pout = queue;
	}

	return event;

}


/**
 * @brief Gets the status of the queue
 * @return Returns the number of pending events in the queue
 */
uint8_t get_Queue_Status()
{
	return number_Of_Elements;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/




/*******************************************************************************
 ******************************************************************************/
