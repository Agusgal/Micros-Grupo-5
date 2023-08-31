/*
 * id_entry.c
 *
 *  Created on: 30 ago. 2023
 *      Author: Agustin Galdeman
 */

#include "encoder_entry.h"

#include "Drivers/Display.h"
#include "Drivers/Encoder.h"

#include "User/userInput.h"

#define ID_SIZE 8

//Variables to keep track of user ID input
static uint8_t id[ID_SIZE];
static uint8_t curr_pos = 0;


void up_number(void)
{
	//increase user input array
	increase_number(id, curr_pos);

	//update display
	writeMessage(id, false);
}

void down_number(void)
{
	//decrease user input array
	decrease_number(id, curr_pos);

	//update display
	writeMessage(id, false);

}

void accept_number(void)
{

}

void msg_fail_encoder(void)
{

}

void msg_ok_encoder(void)
{

}


void init_id()
{
	//give some feedback on user input (leds)
	led_blue_on();

	//reset user input array
	reset_array(id, curr_pos, ID_SIZE);

	//erase screen
	writeMessage("", false);

}
