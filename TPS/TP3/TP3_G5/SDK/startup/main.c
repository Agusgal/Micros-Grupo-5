/*
 * main.c
 *
 *  Created on: May 1, 2015
 *      Author: Juan Pablo VEGA - Laboratorio de Microprocesadores
 */

#include <FTM.h>
#include <gpio.h>
#include <port.h>
#include "hardware.h"


#define __FOREVER__ 	for(;;)

#define PIN_RED_LED 		22     	//PTB22
#define PIN_BLUE_LED 		21     	//PTB21
#define PIN_GREEN_LED 		26 	   	//PTE26
#define PIN_PUSH_BUTTON  	4 		//PTA4


#define BALIZA_DELAY	4000000UL

void idle(void);
void delayLoop (uint32_t veces);

int main (void)
{
	hw_Init ();
	App_Init();

	__FOREVER__
		App_Run();
}





