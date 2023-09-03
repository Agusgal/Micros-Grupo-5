/***************************************************************************//**
  @file     gpio.h
  @brief    Simple GPIO Pin services, similar to Arduino
  @author   Nicolás Magliola
 ******************************************************************************/

#ifndef _FSM_H_
#define _FSM_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#include "../EventQueue/queue.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



#define END_TABLE 0xFF



typedef struct state state;

struct state
{
	void(*funct)(void);
	uint8_t event;
	state *next_state;
};


extern state ID_ENTRY[];
extern state ENCODER_ENTRY[];
extern state CARD_ENTRY[];
extern state PIN_ENTRY[];
extern state RED_LED_ON[];
extern state GREEN_LED_ON[];
//extern state WRONG_ID[];


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/



/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


state* fsm_dispatcher (state* p_state, Event_Type curr_event);

state* get_initial_state(void);

void start_fsm(void);


/*******************************************************************************
 ******************************************************************************/

#endif // _FSM_H_
