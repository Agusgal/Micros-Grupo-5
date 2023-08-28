/***************************************************************************//**
  @file     gpio.h
  @brief    Simple GPIO Pin services, similar to Arduino
  @author   Nicolás Magliola
 ******************************************************************************/

#ifndef _queue_H_
#define _queue_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "MK64F12.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// Queue Status

#define	OVERFLOW	-1
// Events
enum events{
			NONE_EV,
			ENC_LEFT_EV,
			ENC_RIGHT_EV,
			ENC_PRESSED_EV,
			CARD_SWIPE_EV,
			};


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef uint8_t pin_t;
typedef struct Event
{
	uint8_t ID;
} Event_Type;


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Initializes the circular queue
 */
void queue_Init (void);

/**
 * @brief Pushes an event to the queue
 * @param event The element to add to the queue
 * @return Number of pending events. Returns value OVERFLOW if the maximun number of events is reached
 */
uint8_t push_Queue_Element(Event_Type event);

/**
 * @brief Pulls the earliest event from the queue
 * @param queueStatus Pointer to a variable that stores the status of the queue. If no events, OVERFLOW is returned.
 * @return Event_Type variable with the current event if no OVERFLOW is detected.
 */
Event_Type pull_Queue_Element(uint8_t *queueStatus);

/**
 * @brief Gets the status of the queue
 * @return Returns the number of pending events in the queue
 */
uint8_t get_Queue_Status();

/*******************************************************************************
 ******************************************************************************/

#endif // _queue_H_
