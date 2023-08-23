/***************************************************************************//**
  @file     board.h
  @brief    Board management
  @author   Nicolás Magliola
 ******************************************************************************/

#ifndef _BOARD_H_
#define _BOARD_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "gpio.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/***** BOARD defines **********************************************************/

// On Board User LEDs
#define NUM_PINS		100
#define PIN_LED_RED     PORTNUM2PIN(PB,22)
#define PIN_LED_GREEN   PORTNUM2PIN(PE,26)
#define PIN_LED_BLUE    PORTNUM2PIN(PB,21)
#define PIN_SW_EX		PORTNUM2PIN(PC,9)
#define PIN_LEDX		PORTNUM2PIN(PB,23)
#define PIN_SEL0		PORTNUM2PIN(PB,10)
#define PIN_SEL1		PORTNUM2PIN(PB,11)
#define PIN_SEG_A			PORTNUM2PIN(PC,3)
#define PIN_SEG_B			PORTNUM2PIN(PC,2)
#define PIN_SEG_C			PORTNUM2PIN(PA,2)
#define PIN_SEG_D			PORTNUM2PIN(PB,23)
#define PIN_SEG_E			PORTNUM2PIN(PA,1)
#define PIN_SEG_F			PORTNUM2PIN(PB,9)
#define PIN_SEG_G			PORTNUM2PIN(PC,17)
#define PIN_SEG_DP			PORTNUM2PIN(PC,16)
#define PIN_CH_A			PORTNUM2PIN(PC,11)
#define PIN_CH_B			PORTNUM2PIN(PC,10)


#define LED_ACTIVE      LOW


// On Board User Switches
#define PIN_SW2         PORTNUM2PIN(PC,6)
#define PIN_SW3         PORTNUM2PIN(PA,4)

#define SW_ACTIVE       LOW
#define SW_INPUT_TYPE


/*******************************************************************************
 ******************************************************************************/

#endif // _BOARD_H_
