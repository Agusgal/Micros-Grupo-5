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

#include "pin_entry.h"

#define PIN_SIZE 4

//Variables to keep track of user PIN input
static int8_t pin[PIN_SIZE];
static uint8_t curr_pos = 0;


void pin_accept_number(void)
{
	bool is_last = input_pin_number(pin, &curr_pos, PIN_SIZE);

	writeMessage(pin,false);
	for(int i = 0; i < curr_pos - 3; i++)
	{
		ScrollRightOnce();
	}

	if (is_last && curr_pos >= 3)
	{
		pin[PIN_SIZE - 1] = '\0';
		bool pin_ok = check_pin(pin);

		if (pin_ok)
		{
			push_Queue_Element(PIN_OK_EV);
		}
		else
		{
			push_Queue_Element(PIN_FAIL_EV);
		}
	}
	else if (is_last && curr_pos < 3)
	{
		push_Queue_Element(PIN_SHORT_EV);
	}

	//if every number was written the we check if the id is valid
	/*if (curr_pos > 3)
	{
		pin[ID_SIZE - 1] = '\0';
		bool id_ok = check_encoder_id(id);

		if (id_ok)
		{
			push_Queue_Element(ID_OK_ENC_EV);
		}
		else
		{
			push_Queue_Element(ID_FAIL_ENC_EV);
		}
	}*/
}

void pin_down_number(void)
{
	decrease_pin_number(pin, curr_pos);

	//update display
	writeMessage(pin,false);
	for(int i = 0; i < curr_pos - 3; i++)
	{
		ScrollRightOnce();
	}
	writeMessage(pin, false);
}

void pin_up_number(void)
{
	//increase user input array
	increase_pin_number(pin, curr_pos);

	//update display
	writeMessage(pin,false);
	for(int i = 0; i < curr_pos - 3; i++)
	{
		ScrollRightOnce();
	}
	writeMessage(pin, false);
}


void msg_pin_short(void)
{
	writeMessage("Pin too short", true);
}

void msg_fail_pin(void)
{
	writeMessage("Pin failed", true);
}
void msg_ok_pin(void)
{
	writeMessage("Pin ok", true);
}

void init_pin()
{
	reset_array(pin, curr_pos, PIN_SIZE);
	writeMessage("Correct card ID", true);
}
