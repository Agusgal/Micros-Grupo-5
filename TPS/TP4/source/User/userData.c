/*
 * userData.c
 *
 *  Created on: 1 sep. 2023
 *      Author: Grupo 5
 */

#include "userData.h"
#include <string.h>

#define MAX_USER_COUNT	6

static User user_array[MAX_USER_COUNT] = {{"77777777",  "7777",  1, false, {2, 5, 1, 7, 6, 6, 0, 1, 3, 0, 7, 5, 9, 0, 8, 6, 99, 99, 99}},
		                                 {"12345678" , "1234" , 1, false, {4, 5, 1, 7, 6, 6, 0, 1, 3, 0, 7, 5, 9, 0, 8, 6, 99, 99, 99}},
		                                 {"99999999",  "99999", 2, false, {4, 5, 4, 0, 7, 5, 0, 0, 7, 1, 5,8 , 0, 0, 8, 7, 99, 99, 99}},
										 {"11111111", "11111", 2, false, {4, 5, 0, 9, 7, 9, 0, 2, 1, 5, 3, 3, 0, 0, 5, 5, 99, 99, 99}},
										 {"10101010",  "10101", 3, false, {5, 5, 4, 7, 3, 0, 0, 9, 5, 1, 7, 2, 7, 9, 9, 2, 99, 99, 99}},
										 {"66666666",  "66666", 3, false, {4, 1, 1, 1, 9, 7, 2, 7, 2, 4, 4, 1, 8, 8, 8, 2, 99, 99, 99}}};

static int selected_user;

bool check_encoder_id(char *id)
{
	bool found_id = false;
	for (int i = 0; i < MAX_USER_COUNT; i++)
	{
		if (!strcmp(id, user_array[i].id))
		{
			found_id =  true;
			selected_user = i;
		}
	}
	return found_id;
}


bool check_card_id (uint8_t *id)
{
	bool found_card_id = false;
	bool exit_for = false;

	for (int i = 0; i < MAX_USER_COUNT && !exit_for; i++)
	{
		found_card_id = true;
		for (int j = 0; j < 19; j++)
		{
			if (id[j] != user_array[i].card_id[j])
			{
				found_card_id = false;
			}
		}

		if(found_card_id)
		{
			selected_user = i;
			exit_for = true;
		}
	}


	return found_card_id;
}

//todo modificar para que no sea necesario hacer esto
bool check_pin(char *pin)
{
	if (pin[4] == '$')
	{
		pin[4] = '\0';
	}

	if (!strcmp(pin, user_array[selected_user].pin))
	{
		return true;
	}
	else
	{
		return false;
	}
}

int getSelectedUSer(void)
{
	return selected_user;
}

int getFloor(int user)
{
	return user_array[user].floor;
}

bool isInside(int user)
{
	return user_array[user].inBuilding;
}

void enterBuilding(int user)
{
	user_array[user].inBuilding = true;
}

void exitBuilding(int user)
{
	user_array[user].inBuilding = false;
}


