/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "board.h"
#include "gpio.h"
#include <stdio.h>
#include "MK64F12.h"
#include "hardware.h"
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
enum encoderStates {ACW1=1,ACW2,ACW3,ACW4,CW1,CW2,CW3,CW4};
enum events {ANTI_CLOCKWISE_TURN, CLOCKWISE_TURN};
/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 *******************************************************************************/

void IRQHandler(uint8_t event)
{
	static int number=0;
	switch(event)
	{
	case(CLOCKWISE_TURN):
			number++;
			updateDisplay(number);
			break;
	case(ANTI_CLOCKWISE_TURN):
			number--;
			updateDisplay(number);
			break;
	}

}

__ISR__ PORTC_IRQHandler(void)
{
	NVIC_DisableIRQ(PORTC_IRQn);
	int port_isr=0;
	static int state = IDLE;

	while ((port_isr<32) && !(PORTC->PCR[port_isr] & (PORT_PCR_ISF_MASK)))
	{
		port_isr++;
	}
	PORTC->PCR[port_isr] |= PORT_PCR_ISF_MASK;
	bool CH_A=(PIN2NUM(PIN_CH_A)==port_isr);
	switch (state)
	{
		case IDLE:
			if (CH_A)
				state=ACW1;
			else
				state=CW1;
			break;
		case ACW1:
			if (CH_A)
				state=IDLE;
			else
				state=ACW2;
			break;
		case ACW2:
			if (CH_A)
				state=ACW3;
			else
				state=ACW1;
			break;
		case ACW3:
			if (CH_A)
				state=ACW1;
			else
				state=CW1;
			break;
		case ACW4:
				state=IDLE;
				IRQHandler(ANTI_CLOCKWISE_TURN);
			break;
		case CW1:
			if (CH_A)
				state=CW2;
			else
				state=IDLE;
			break;
		case CW2:
			if (CH_A)
				state=CW1;
			else
				state=CW3;
			break;
		case CW3:
			if (CH_A)
				state=CW4;
			else
				state=CW2;
			break;
		case CW4:
			state=IDLE;
			IRQHandler(CLOCKWISE_TURN);
			break;
	}
	NVIC_EnableIRQ(PORTC_IRQn);

}


__ISR__ SysTick_Handler(void)
{
	updateDisplay(-1);
}

void SysTick_Init (void)
{
	SysTick->CTRL = 0x00;
	SysTick->LOAD = 124999L; //12499999L; // <= 125 ms @ 100Mhz
	SysTick->VAL  = 0x00;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}
/*******************************************************************************
 ******************************************************************************/
