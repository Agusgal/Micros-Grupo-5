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
#include "Timers.h"
#include "BoardLeds.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define IDLE 0
#define RISING_FLANK 2
#define SYSTICK_ISR_PERIOD_US 500U

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static int green_cont;
static bool green_turned_off = false;

static int red_cont;
static bool red_turned_off = false;

static void green_led_timer_pisr(void);
static void red_led_timer_pisr(void);

//Def del timer
static tim_id_t green_led_timer;
static tim_id_t red_led_timer;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


void BoardLeds_Init(void)
{
	initTimers();
	green_led_timer = timerGetId();
	red_led_timer = timerGetId();

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


	timerStart(green_led_timer, TIMER_MS2TICKS(1), TIM_MODE_PERIODIC, green_led_timer_pisr);
	timerStart(red_led_timer, TIMER_MS2TICKS(1), TIM_MODE_PERIODIC, red_led_timer_pisr);
}

void led1On()
{
	gpioWrite(PIN_ST_0,0);
	gpioWrite(PIN_ST_1,1);
}

void led2On()
{
	gpioWrite(PIN_ST_0,1);
	gpioWrite(PIN_ST_1,0);
}

void led3On()
{
	gpioWrite(PIN_ST_0,1);
	gpioWrite(PIN_ST_1,1);
}

void ledOff()
{
	gpioWrite(PIN_ST_0,0);
	gpioWrite(PIN_ST_1,0);
}




void led_red_on()
{
	gpioWrite(PIN_LED_RED,0);
	led3On();
}

void led_green_on()
{
	gpioWrite(PIN_LED_GREEN,0);
	led1On();
}

void led_blue_on()
{
	gpioWrite(PIN_LED_BLUE,0);
}

void led_toggle(ledID ledId)
{
	gpioToggle(ledId);
}


void led_green_on_time(uint32_t time)
{
	led_green_on();
	green_cont = time/SYSTICK_ISR_PERIOD_US;
}

bool get_green_status()
{
	if (green_turned_off)
	{
		green_turned_off = false;
		return true;
	}
	else
	{
		return false;
	}
}

void led_red_on_time(uint32_t time)
{
	led_red_on();
	red_cont = time/SYSTICK_ISR_PERIOD_US;
}

bool get_red_status(void)
{
	if (red_turned_off)
	{
		red_turned_off = false;
		return true;
	}
	else
	{
		return false;
	}
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


static void green_led_timer_pisr()
{
	if (green_cont)
	{
		green_cont--;
		if (!green_cont)
		{
			gpioWrite(PIN_LED_GREEN, 1);
			green_turned_off = true;
		}
	}
}

static void red_led_timer_pisr()
{
	if (red_cont)
	{
		red_cont--;
		if (!red_cont)
		{
			gpioWrite(PIN_LED_RED, 1);
			red_turned_off = true;
		}
	}
}
/*******************************************************************************
 ******************************************************************************/
