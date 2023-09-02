/*
 * id_entry.c
 *
 *  Created on: 30 ago. 2023
 *      Author: Agustin Galdeman
 */

#include "id_entry.h"
#include "Drivers/CardReader_DRV.h"
#include "Drivers/BoardLeds.h"
#include "EventQueue/queue.h"
#include "card_entry.h"

#include "User/userInput.h"
#include "User/userData.h"

#define ARRAY_END 99

static uint8_t card_id_raw[NUMBER_OF_CHARACTERS];
static uint8_t card_id[19];

static void initialize_id_array(void);


void init_cardswipe(void)
{
	initialize_id_array();
	getCardReader_Data(card_id_raw);

	if(!checkLRC_CardReader_Data(card_id_raw, NUMBER_OF_CHARACTERS))
	{
		push_Queue_Element(RETURN_EV);
	}
	else if(getCard_ID(card_id_raw, NUMBER_OF_CHARACTERS, card_id))
	{
		if (check_card_id (card_id))
		{
			push_Queue_Element(ID_OK_CARD_EV);
		}
		else
		{
			push_Queue_Element(ID_FAIL_CARD_EV);
		}
	}
	else
	{
		push_Queue_Element(RETURN_EV);
	}

}



static void initialize_id_array(void)
{
	for(int i =0; i < 19; i++)
	{
		card_id[i] = ARRAY_END;
	}
}

void msg_fail_card()
{
	writeMessage("id failed", true);
}

void msg_ok_card()
{
	writeMessage("correct id",true);
}

