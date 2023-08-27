/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <IRQ.h>
#include "board.h"
#include "gpio.h"
#include <stdio.h>
#include "MK64F12.h"
#include "hardware.h"
#include "Encoder.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void idle(void);
void SysTick_Init (void);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 *******************************************************************************/

void IRQHandler(uint8_t event)
{
	static int number=0;
	static int selected_digit=0;
	static int state=0;
	int multiplier=1;
	for (int i=selected_digit;i<3;i++)
	{
		multiplier*=10;
	}
	switch(event)
	{
	case(CLOCKWISE_TURN):
		if(state==0)
		{
			number+=multiplier;
			updateDisplay(number);
		}
		else if (state==1)
		{
			selected_digit++;
			selected_digit=selected_digit%4;
		}
		break;
	case(ANTI_CLOCKWISE_TURN):
		if(state==0)
		{
			number-=multiplier;
			updateDisplay(number);
		}
		else if (state==1)
		{
			selected_digit--;
			selected_digit=selected_digit%4;
		}
		break;
	}

}



__ISR__ SysTick_Handler(void)
{
	updateDisplay(-1);
	static int counter=0;
	counter++;
	if (counter==10)
	{
		IRQHandler(EncoderStatus());
		counter=0;
	}

}

void SysTick_Init (void)
{
	SysTick->CTRL = 0x00;
	SysTick->LOAD = 100000L-1; //1 ms
	SysTick->VAL  = 0x00;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}


/*******************************************************************************
 ******************************************************************************/
