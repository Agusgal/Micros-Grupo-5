/*
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    gpio.c
 * @brief   Application entry point.
 */
#include "gpio.h"
#include <stdio.h>
#include "board.h"
#include "MK64F12.h"
//#include "fsl_debug_console.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
/*
 * main.c
 *
 *  Created on: August 10, 2023
 *      Author: Daniel Jacoby - Laboratorio de Microprocesadores
 *  Purpose: Show GPIO example
 */



#define __FOREVER__ 	for(;;)

#define PIN_RED_LED 		22     	//PTB22
#define PIN_BLUE_LED 		21     	//PTB21
#define PIN_GREEN_LED 		26 	   	//PTE26
#define PIN_PUSH_BUTTON  	4 		//PTA4





void gpioMode(pin_t pin, uint8_t mode)
{
	uint8_t port=PIN2PORT(pin);
	uint8_t number=PIN2NUM(pin);


	PORT_Type * port_ptr= PORT_PTR(port);
	GPIO_Type * gpio_ptr= GPIO_PTR(port);

	static bool clk_en [5]={0};		// Activamos los registros de clock gating (una vez por puerto)
	if (clk_en[port]==0)
	{
		SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK << port);
		clk_en[port]=1;
	}

	port_ptr->PCR[number]=0x0; 		//Clear
	port_ptr->PCR[number]|=PORT_PCR_MUX(PORT_mGPIO); 		//Set MUX to GPIO


	switch(mode)
	{
		case(INPUT):
			port_ptr->PCR[number]|=PORT_PCR_PE(0);          		//Pull UP/Down Disable
			gpio_ptr->PDDR &= ~(1<<number);
			break;
		case(INPUT_PULLUP):
			port_ptr->PCR[number]|=PORT_PCR_PE(1);          		//Pull UP/Down Enable
			port_ptr->PCR[number]|=PORT_PCR_PS(1);          		//Pull UP
			gpio_ptr->PDDR &= ~(1<<number);
			break;
		case(INPUT_PULLDOWN):
			port_ptr->PCR[number]|=PORT_PCR_PE(1);          		//Pull UP/Down Enable
			port_ptr->PCR[number]|=PORT_PCR_PS(0);          		//Pull Down
			gpio_ptr->PDDR &= ~(1<<number);
			break;
		case(OUTPUT):
			port_ptr->PCR[number]|=PORT_PCR_PE(0);          		//Pull UP/Down Disable
			gpio_ptr->PDDR |= (1<<number);
			port_ptr->GPCHR|=PORT_GPCHR_GPWD(PORT_PCR_DSE(1));
			break;
		case(OUTPUT_PULLDOWN):
			port_ptr->PCR[number]|=PORT_PCR_PE(1);          		//Pull UP/Down
			port_ptr->PCR[number]|=PORT_PCR_PS(0);          		//Pull Down
			gpio_ptr->PDDR |= (1<<number);
			break;
		case(OUTPUT_PULLUP):
			port_ptr->PCR[number]|=PORT_PCR_PE(1);          		//Pull UP/Down  Enable
			port_ptr->PCR[number]|=PORT_PCR_PS(1);          		//Pull UP
			gpio_ptr->PDDR |= (1<<number);
			break;
	}

}

void gpioWrite(pin_t pin, bool value)
{
	uint8_t port=PIN2PORT(pin);
	uint8_t number=PIN2NUM(pin);
	GPIO_Type * gpio_ptr= GPIO_PTR(port);
	if (value)
		gpio_ptr->PSOR=(1<<number);
	else
		gpio_ptr->PCOR=(1<<number);
}

bool gpioRead(pin_t pin)
{
	uint8_t port=PIN2PORT(pin);
	uint8_t number=PIN2NUM(pin);
	GPIO_Type * gpio_ptr= GPIO_PTR(port);
	return gpio_ptr->PDIR & (1<<number);
}

void gpioToggle(pin_t pin)
{
	uint8_t port=PIN2PORT(pin);
	uint8_t number=PIN2NUM(pin);
	GPIO_Type * gpio_ptr= GPIO_PTR(port);
	gpio_ptr->PTOR = (1<<number);
}

void gpioIRQconfig (pin_t pin, PORTEvent_t irq_config)
{
		uint8_t port=PIN2PORT(pin);
		uint8_t number=PIN2NUM(pin);
		PORT_Type * port_ptr= PORT_PTR(port);
		port_ptr->PCR[number]|=PORT_PCR_IRQC(irq_config);
}

bool gpioFlank(pin_t pin, bool active)		// Detector de Flancos en pines de input
{
	static bool state = IDLE;
	bool change=LOW;
	bool pinRead;

	pinRead = gpioRead(pin);

	switch (state)
	{
		case IDLE:
			if (pinRead==active){
				state=PRESSED;
				change=HIGH;
				}
				break;
		case PRESSED:
			if (pinRead==!active)
				state=IDLE;
			break;
	}
	return change;
}



