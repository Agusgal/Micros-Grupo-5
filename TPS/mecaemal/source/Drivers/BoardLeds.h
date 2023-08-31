/***************************************************************************//**
  @file     gpio.h
  @brief    Simple GPIO Pin services, similar to Arduino
  @author   Nicolás Magliola
 ******************************************************************************/

#ifndef _BOARDLEDS_H_
#define _BOARDLEDS_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/


enum encoderStates {ACW1=1,ACW2,ACW3,ACW4,CW1,CW2,CW3,CW4};
enum events {ANTI_CLOCKWISE_TURN=1, CLOCKWISE_TURN,ENC_SW_PRESS};
typedef enum ledID {RED_LED = PIN_LED_RED , BLUE_LED = PIN_LED_BLUE, GREEN_LED = PIN_LED_GREEN} ledID;


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

void BoardLeds_Init(void);

void led1On ();
void led2On ();
void led3On ();
void led_blue_on();
void led_green_on();
void led_red_on();
void led_toggle(ledID ledId);
/*******************************************************************************
 ******************************************************************************/

#endif // _BOARDLEDS_H_
