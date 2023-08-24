/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "board.h"
#include "IRQ.h"
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

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 *******************************************************************************/

void IRQHandler(uint8_t event)
{
	static int number=0;
	static int selected_digit=0;
	static int state=SELECTING_NUMBER;
	int multiplier=1;
	for (int i=selected_digit;i<3;i++)
	{
		multiplier*=10;
	}
	switch(event)
	{
	case(CLOCKWISE_TURN):
		if(state==SELECTING_NUMBER)
		{
			number+=multiplier;
			updateDisplay(number);
		}
		else if (state==SELECTING_DIGIT)
		{
			selected_digit++;
			selected_digit=selected_digit%4;
		}
		break;
	case(ANTI_CLOCKWISE_TURN):
		if(state==SELECTING_NUMBER)
		{
			number-=multiplier;
			updateDisplay(number);
		}
		else if (state==SELECTING_DIGIT)
		{
			selected_digit--;
			selected_digit=selected_digit%4;
		}
		break;
	case(SW2_PRESS):
		if (state==SELECTING_DIGIT)
			state=SELECTING_NUMBER;
		else if (state==SELECTING_NUMBER)
			state=SELECTING_DIGIT;
		break;
	}

}

__ISR__ PORTC_IRQHandler(void)
{

	// Código para detectar el pin que provocó la interrupción
	int port_isr=32;
	static int state = IDLE;
	while ((port_isr>=0) && !(PORTC->PCR[port_isr] & (PORT_PCR_ISF_MASK)))
	{
		port_isr--;
	}

	PORTC->PCR[port_isr] |= PORT_PCR_ISF_MASK;

	bool CH_A=gpioRead(PIN_CH_A);
	bool CH_B=gpioRead(PIN_CH_B);
	bool SW_2=gpioRead(PIN_SW2);
	/*****************************************/
	// Máquina de Estados
	if (SW_2==!SW_ACTIVE)
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
					state=IDLE;
				else if (!CH_A & !CH_B)
					state=ACW2;
				else
					state=ACW1;
				break;
			case ACW2:
				if (CH_A & !CH_B)
					state=ACW3;
				else if (!CH_A & CH_B)
					state=ACW1;
				else
					state=ACW2;
				break;
			case ACW3:
				if (CH_A & CH_B)
					state=ACW4;
				else if (CH_A & !CH_B)
					state=ACW3;
				else
					state=ACW3;
				break;
			case ACW4:
					state=IDLE;
					IRQHandler(ANTI_CLOCKWISE_TURN);	//Hubo un anticlockwise turn
				break;
			case CW1:
				if (!CH_A & !CH_B)
					state=CW2;
				else if (CH_A & CH_B)
					state=IDLE;
				else
					state=CW1;
				break;
			case CW2:
				if (!CH_A & CH_B)
					state=CW3;
				else if (CH_A & !CH_B)
					state=CW1;
				else if (CH_A & CH_B)
				{
					state=IDLE;
					IRQHandler(CLOCKWISE_TURN);
				}
				else
					state=CW2;
				break;
			case CW3:

				if (CH_A & CH_B)
				{
					state=IDLE;
					IRQHandler(CLOCKWISE_TURN);

				}
				else if (!CH_A & !CH_B)
					state=CW2;
				else
					state=CW3;
				break;

		}
	else
		IRQHandler(SW2_PRESS);
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
