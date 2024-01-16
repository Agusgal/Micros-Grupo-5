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

#include "../board.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

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

void ledOff ();

void led_blue_on();
void led_green_on();
void led_red_on();

void led_toggle(ledID ledId);

void led_green_on_time(uint32_t time);
bool get_green_status(void);

void led_red_on_time(uint32_t time);
bool get_red_status(void);

/*******************************************************************************
 ******************************************************************************/

#endif // _BOARDLEDS_H_
