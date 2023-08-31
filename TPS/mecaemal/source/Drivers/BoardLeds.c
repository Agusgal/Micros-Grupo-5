/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/



#include "gpio.h"
#include <stdio.h>
#include "board.h"
#include "Encoder.h"
#include "Systick.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define IDLE 0
#define RISING_FLANK 2


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


void BoardLeds_Init(void)
{
	gpioMode(PIN_ST_0,OUTPUT);
	gpioMode(PIN_ST_1,OUTPUT);

	gpioWrite(PIN_ST_0,0);
	gpioWrite(PIN_ST_1,0);


	gpioMode(PIN_LED_RED,OUTPUT);
	gpioMode(PIN_LED_GREEN,OUTPUT);
	gpioMode(PIN_LED_BLUE,OUTPUT);

	gpioWrite(PIN_LED_RED,1);
	gpioWrite(PIN_LED_GREEN,1);
	gpioWrite(PIN_LED_BLUE,1);


}

void led1On ()
{
	gpioWrite(PIN_ST_0,0);
	gpioWrite(PIN_ST_1,1);
}
void led2On ()
{
	gpioWrite(PIN_ST_0,1);
	gpioWrite(PIN_ST_1,0);
}
void led3On ()
{
	gpioWrite(PIN_ST_0,1);
	gpioWrite(PIN_ST_1,1);

}

void led_red_on()
{
	gpioWrite(PIN_LED_RED,0);
}

void led_green_on()
{
	gpioWrite(PIN_LED_GREEN,0);
}

void led_blue_on()
{
	gpioWrite(PIN_LED_BLUE,0);
}

void led_toggle(ledID ledId)
{
	gpioToggle(ledId);
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



/*******************************************************************************
 ******************************************************************************/
