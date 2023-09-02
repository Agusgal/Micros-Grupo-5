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
#include "Drivers/BoardLeds.h"

#include "EventQueue/queue.h"

#include "User/userData.h"

#define ID_SIZE 9

//Variables to keep track of user ID input
static int8_t id[ID_SIZE];
static uint8_t curr_pos = 0;


void up_number(void)
{
	//increase user input array
	increase_number(id, curr_pos);

	//update display
	pauseMessage();
	writeMessage(id, false);
	for(int i = 0; i < curr_pos - 3; i++)
	{
		ScrollRightOnce();
	}
}

void down_number(void)
{
	//decrease user input array
	decrease_number(id, curr_pos);

	//update display
	pauseMessage();
	writeMessage(id, false);
	for(int i = 0; i < curr_pos-3; i++)
	{
		ScrollRightOnce();
	}

}

void accept_number(void)
{
	input_number(id, &curr_pos,ID_SIZE);

	writeMessage(id,false);
	for(int i = 0; i < curr_pos - 3; i++)
	{
		ScrollRightOnce();
	}

	//if every number was written the we check if the id is valid
	if (curr_pos > 7 )
	{
		id[ID_SIZE - 1] = '\0';
		bool id_ok = check_encoder_id(id);

		if (id_ok)
		{
			push_Queue_Element(ID_OK_ENC_EV);
		}
		else
		{
			push_Queue_Element(ID_FAIL_ENC_EV);
		}
	}

}


void msg_fail_encoder(void)
{
	writeMessage("ID not found", true);
}

void msg_ok_encoder(void)
{
	writeMessage("ID found", true);
}


void init_id()
{
	//give some feedback on user input (leds). Toggles LED until correct ID is found
	led_toggle(BLUE_LED);

	//reset user input array
	reset_array(id, &curr_pos, ID_SIZE);

	//erase screen
	pauseMessage();

}
