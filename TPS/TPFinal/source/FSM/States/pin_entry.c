/*
 * id_entry.c
 *
 *  Created on: 30 ago. 2023
 *      Author: Agustin Galdeman
 */

#include "encoder_entry.h"

#include "Drivers/Legacy/Display.h"
#include "Drivers/HAL/Encoder.h"

#include "User/userInput.h"
#include "Drivers/Legacy/BoardLeds.h"

#include "EventQueue/queue.h"

#include "User/userData.h"

#include "pin_entry.h"

#define PIN_SIZE 6

//Variables to keep track of user PIN input
static int8_t pin[PIN_SIZE];
static uint8_t curr_pos = 0;

static int strike3 = 0;

static void write_hidden(void);


void pin_accept_number(void)
{
	bool is_last = input_pin_number(pin, &curr_pos, PIN_SIZE, &strike3);

	write_hidden();
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
			strike3 = 0;
		}
		else
		{
			strike3++;
			if (strike3 == 3)
			{
				push_Queue_Element(PIN_3_TIMES_EV);
				strike3 = 0;
			}
			else
			{
				reset_array(pin, &curr_pos, PIN_SIZE);
				push_Queue_Element(PIN_FAIL_EV);
			}
		}
	}
	else if (is_last && curr_pos < 3)
	{
		reset_array(pin, &curr_pos, PIN_SIZE);
		push_Queue_Element(PIN_SHORT_EV);
	}
}

void pin_down_number(void)
{
	decrease_pin_number(pin, curr_pos);

	//update display
	write_hidden();
	//writeMessage(pin,false);
	for(int i = 0; i < curr_pos - 3; i++)
	{
		ScrollRightOnce();
	}
	//writeMessage(pin, false);
}

void pin_up_number(void)
{
	//increase user input array
	increase_pin_number(pin, curr_pos);

	//update display
	write_hidden();
	//writeMessage(pin,false);
	for(int i = 0; i < curr_pos - 3; i++)
	{
		ScrollRightOnce();
	}
	//writeMessage(pin, false);
}


void msg_pin_short(void)
{
	writeMessage("Pin too short", true);
}

void msg_fail_pin(void)
{
	writeMessage("Pin failed", true);
}

void msg_pin_3_times(void)
{
	writeMessage("Pin failed 3 tines", true);
	led_red_on_time(5000000U);
}



void msg_ok_pin(void)
{
	writeMessage("Pin ok", true);
	led_green_on_time(5000000U);
}

void init_pin()
{
	writeMessage("Correct card ID insert pin", true);
	reset_array(pin, &curr_pos, PIN_SIZE);
}


void msg_fail_encoder(void)
{
	writeMessage("ID not found", true);
	led_red_on_time(5000000U);
}

void msg_ok_encoder(void)
{
	writeMessage("ID found", true);
	reset_array(pin, &curr_pos, PIN_SIZE);
}


static void write_hidden()
{
	int8_t pin_copy[PIN_SIZE];

	for (int i = 0; i < PIN_SIZE; i++)
	{
		if (i != curr_pos)
		{
			pin_copy[i] = '-';
		}
		else
		{
			pin_copy[i] = pin[i];
		}
	}

	writeMessage(pin_copy, false);
}

