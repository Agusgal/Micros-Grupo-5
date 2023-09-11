/***************************************************************************//**
  @file     queue.h
  @brief    Event queue impelmentation
  @author   Bruno Di Sanzo, Agustin Galdeman
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

typedef enum {
			NONE_EV,
			ENC_LEFT_EV,
			ENC_RIGHT_EV,
			ENC_PRESSED_EV,
			CARD_SWIPE_EV,
			CARD_MIDSWIPE_EV,
			INCREASE_BRIGHTNESS_EV,
			RETURN_EV,

			ID_FAIL_ENC_EV,
			ID_FAIL_CARD_EV,

			ID_OK_ENC_EV,
			ID_OK_CARD_EV,

			PIN_FAIL_EV,
			PIN_OK_EV,
			PIN_SHORT_EV,
			PIN_3_TIMES_EV,

			FIVE_SEC_LAPSE_EV,
			MIN_LAPSE_EV

			}Event_Type;


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef uint8_t pin_t;

//typedef struct Event
//{
	//uint8_t ID;
//} Event_Type;


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
int8_t push_Queue_Element(Event_Type event);

/**
 * @brief Pulls the earliest event from the queue
 * @return Event_Type variable with the current event if no OVERFLOW is detected.
 */
Event_Type pull_Queue_Element(void);

/**
 * @brief Gets the status of the queue
 * @return Returns the number of pending events in the queue
 */
uint8_t get_Queue_Status();



/*******************************************************************************
 ******************************************************************************/

#endif // _queue_H_
