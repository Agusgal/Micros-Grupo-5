/***************************************************************************//**
  @file     gpio.h
  @brief    Simple GPIO Pin services, similar to Arduino
  @author   Nicolás Magliola
 ******************************************************************************/

#ifndef _ENCODER_H_
#define _ENCODER_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#include "../EventQueue/queue.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

<<<<<<<< HEAD:TPS/FSMTP1/source/FSM/FSM.h

#define END_TABLE 0xFF



typedef struct state state;

struct state
{
	void(*funct)(void);
	uint8_t event;
	state *next_state;
};


========
>>>>>>>> tests-jp:TPS/FSMTP1/source/Drivers/BoardLeds.h

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
<<<<<<<< HEAD:TPS/FSMTP1/source/FSM/FSM.h
extern state ID_ENTRY[];
extern state ENCODER_ENTRY[];
extern state CARD_ENTRY[];
extern state PIN_ENTRY[];
extern state RED_LED_ON[];
extern state GREEN_LED_ON[];
extern state BRIGHTNESS[];
extern state WRONG_ID[];
========

enum encoderStates {ACW1=1,ACW2,ACW3,ACW4,CW1,CW2,CW3,CW4};
enum events {ANTI_CLOCKWISE_TURN=1, CLOCKWISE_TURN,ENC_SW_PRESS};

>>>>>>>> tests-jp:TPS/FSMTP1/source/Drivers/BoardLeds.h


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

<<<<<<<< HEAD:TPS/FSMTP1/source/FSM/FSM.h
state* fsm_dispatcher (state* p_state, Event_Type curr_event);

state* get_initial_state(void);

void start_fsm(void);
========
void BoardLeds_Init(void);

void led1On ();
void led2On ();
void led3On ();
>>>>>>>> tests-jp:TPS/FSMTP1/source/Drivers/BoardLeds.h

/*******************************************************************************
 ******************************************************************************/

#endif // _GPIO_H_
