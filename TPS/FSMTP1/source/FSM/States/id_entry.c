/*
 * id_entry.c
 *
 *  Created on: 30 ago. 2023
 *      Author: Agustin Galdeman
 */

#include "id_entry.h"

#include "Drivers/Display.h"
#include "Drivers/Encoder.h"
#include "Drivers/BoardLeds.h"

#define ID_SIZE 8


//Variables to keep track of user ID input
static uint8_t id[ID_SIZE];
static uint8_t curr_pos = 0;



void init_id()
{
	//give some feedback on user input (leds)
	led_blue_on();

	//stop display multiplexing (animations)
	pauseMessage();

}

void init_cardswipe(void)
{
	//?
}


void welcome_animation()
{
	writeMessage("Welcome", true);
}
