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
#include "MK64F12.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/* States
enum states{
			ID_ENTRY,
			ENCODER_ENTRY,
			CARD_ENTRY,
			PIN_ENTRY,
			RED_LED_ON,
			GREEN_LED_ON,
			BRIGHTNESS,
			WRONG_ID};
*/

typedef struct state state;

struct state {void(*funct)(void);
				uint8_t event;
				state *next_state;};



#define END_TABLE 0xFF


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
extern state ID_ENTRY[];
extern state ENCODER_ENTRY[];
extern state CARD_ENTRY[];
extern state PIN_ENTRY[];
extern state RED_LED_ON[];
extern state GREEN_LED_ON[];
extern state BRIGHTNESS[];
extern state WRONG_ID[];


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

state* fsm_dispatcher (state* p_state, uint8_t curr_event, uint8_t next_state);

/*******************************************************************************
 ******************************************************************************/

#endif // _FSM_H_
