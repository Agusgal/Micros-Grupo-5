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

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/



/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


void Encoder_Init(void)
{
	gpioMode(PIN_CH_A,INPUT);
	gpioMode(PIN_CH_B,INPUT);
	gpioMode(PIN_DEC_SW,INPUT);
}

int EncoderStatus(void)
{
	static int state=IDLE;
	bool CH_A=gpioRead(PIN_CH_A);
	bool CH_B=gpioRead(PIN_CH_B);

	/*****************************************/
	// Máquina de Estados

	switch (state)
	{
		case IDLE:
			if (!CH_A & CH_B)
				state=ACW1;
			else if (CH_A & !CH_B)
				state=CW1;
			else
				state=IDLE;
			break;
		case ACW1:
			if (CH_A & CH_B)
				state=ACW3;
			else if (!CH_A & !CH_B)
				state=ACW2;
			else
				state=ACW1;
			break;
		case ACW2:
			if (!CH_A & CH_B)
				state=ACW1;
			else if (!CH_A & !CH_B)
				state=ACW2;
			else
				state=ACW3;
			break;
		case ACW3:
			if (CH_A & CH_B)
			{
				state=IDLE;
				return ANTI_CLOCKWISE_TURN;
			}
			else if (!CH_A & !CH_B)
				state=ACW2;
			else
				state=ACW3;
			break;
		case CW1:
			if (!CH_A & !CH_B)
				state=CW2;
			else if (CH_A & CH_B)
				state=CW3;
			else
				state=CW1;
			break;
		case CW2:
			if (CH_A & !CH_B)
				state=CW1;
			else if (!CH_A & !CH_B)
				state=CW2;
			else
				state=CW3;
			break;
		case CW3:
			if (CH_A & CH_B)
			{
				state=IDLE;
				return CLOCKWISE_TURN;
			}
			else if (!CH_A & !CH_B)
				state=CW2;
			else
				state=CW3;
			break;
	}
	return IDLE;

}

char EncoderSwitchRead(void)
{
	static int sw_state = IDLE;

	bool sw_Read=gpioRead(PIN_DEC_SW);
	switch (sw_state)
	{
		case IDLE:
			if (sw_Read==HIGH)
				return RISING_FLANK;
			else
				return LOW;
			break;
		case PRESSED:
			if (sw_Read==LOW)
			{
				sw_state=IDLE;
				return LOW;
			}
			else
				return HIGH;
			break;
	}
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



/*******************************************************************************
 ******************************************************************************/
