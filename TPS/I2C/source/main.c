/*
 * main.c
 *
 *  Created on: May 1, 2015
 *      Author: Juan Pablo VEGA - Laboratorio de Microprocesadores
 */

#include <i2c.h>
#include "hardware.h"

#define __FOREVER__ 	for(;;)

#define PIN_RED_LED 		22     	//PTB22
#define PIN_BLUE_LED 		21     	//PTB21
#define PIN_GREEN_LED 		26 	   	//PTE26
#define PIN_PUSH_BUTTON  	4 		//PTA4

typedef enum
{
	PORT_mAnalog,
	PORT_mGPIO,
	PORT_mAlt2,
	PORT_mAlt3,
	PORT_mAlt4,
	PORT_mAlt5,
	PORT_mAlt6,
	PORT_mAlt7,

} PORTMux_t;

typedef enum
{
	PORT_eDisabled				= 0x00,
	PORT_eDMARising				= 0x01,
	PORT_eDMAFalling			= 0x02,
	PORT_eDMAEither				= 0x03,
	PORT_eInterruptDisasserted	= 0x08,
	PORT_eInterruptRising		= 0x09,
	PORT_eInterruptFalling		= 0x0A,
	PORT_eInterruptEither		= 0x0B,
	PORT_eInterruptAsserted		= 0x0C,
} PORTEvent_t;

#define BALIZA_DELAY	4000000UL

void idle(void);
void delayLoop (uint32_t veces);

int main (void)
{

}


__ISR__  PORTA_IRQHandler(void)
{

}


void idle(void)
{

}


void delayLoop (uint32_t veces)
{
	while (veces--);
}


__ISR__  SysTick_Handler (void)
{
	static uint32_t speed=4;  // 0.5 seg @ tick =125ms

	if (speed==0)
	{

		PTB->PTOR = (1<<21)|(1<<22);
		speed=4;

	}

	speed--;


}

void SysTick_Init (void)
{
	SysTick->CTRL = 0x00;
	SysTick->LOAD = 12500000L-1; //12499999L; // <= 125 ms @ 100Mhz
	SysTick->VAL  = 0x00;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}


