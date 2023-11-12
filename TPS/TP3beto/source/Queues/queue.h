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

#define	MAX_NUMBER_OF_ELEMENTS	64

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct queue_uint8
{
	uint8_t queue [MAX_NUMBER_OF_ELEMENTS];
	uint8_t num_Of_Events;
	uint8_t *pin, *pout;

}queue_uint8_t;

typedef struct queue_uint16
{
	uint16_t queue [MAX_NUMBER_OF_ELEMENTS];
	uint8_t num_Of_Events;
	uint16_t *pin, *pout;

}queue_uint16_t;

typedef struct queue_uint32
{
	uint32_t queue [MAX_NUMBER_OF_ELEMENTS];
	uint8_t num_Of_Events;
	uint32_t *pin, *pout;

}queue_uint32_t;


typedef uint8_t pin_t;


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


/**
 * @brief Initializes the circular queue
 */
void queue_Init_uint8(queue_uint8_t* buffer);


/**
 * @brief Pushes an event to the queue
 * @param event The element to add to the queue
 * @return Number of pending events. Returns value OVERFLOW if the maximun number of events is reached
 */
int8_t push_Queue_Element_uint8(queue_uint8_t* buffer, uint8_t event);


/**
 * @brief Pulls the earliest event from the queue
 * @return Event_Type variable with the current event if no OVERFLOW is detected.
 */
uint8_t pull_Queue_Element_uint8(queue_uint8_t* buffer);


/**
 * @brief Gets the status of the queue
 * @return Returns the number of pending events in the queue
 */
uint8_t get_Queue_Status_uint8(queue_uint8_t* buffer);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Initializes the circular queue
 */
void queue_Init_uint16(queue_uint16_t* buffer);


/**
 * @brief Pushes an event to the queue
 * @param event The element to add to the queue
 * @return Number of pending events. Returns value OVERFLOW if the maximun number of events is reached
 */
int8_t push_Queue_Element_uint16(queue_uint16_t* buffer, uint16_t event);


/**
 * @brief Pulls the earliest event from the queue
 * @return Event_Type variable with the current event if no OVERFLOW is detected.
 */
uint16_t pull_Queue_Element_uint16(queue_uint16_t* buffer);


/**
 * @brief Gets the status of the queue
 * @return Returns the number of pending events in the queue
 */
uint8_t get_Queue_Status_uint16(queue_uint16_t* buffer);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Initializes the circular queue
 */
void queue_Init_uint32(queue_uint32_t* buffer);


/**
 * @brief Pushes an event to the queue
 * @param event The element to add to the queue
 * @return Number of pending events. Returns value OVERFLOW if the maximun number of events is reached
 */
int8_t push_Queue_Element_uint32(queue_uint32_t* buffer, uint32_t event);


/**
 * @brief Pulls the earliest event from the queue
 * @return Event_Type variable with the current event if no OVERFLOW is detected.
 */
uint32_t pull_Queue_Element_uint32(queue_uint32_t* buffer);


/**
 * @brief Gets the status of the queue
 * @return Returns the number of pending events in the queue
 */
uint8_t get_Queue_Status_uint32(queue_uint32_t* buffer);



/*******************************************************************************
 ******************************************************************************/

#endif // _queue_H_
