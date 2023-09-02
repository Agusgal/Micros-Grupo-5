/*
 * userData.c
 *
 *  Created on: 1 sep. 2023
 *      Author: agus
 */

#include "userData.h"
#include <string.h>


char *user_id1 = "11111111";
char *user_pin1 = "1234";

bool check_encoder_id(char *id)
{
	if (!strcmp(id, user_id1))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool check_card_id (uint8_t *id)
{
	for (int i = 0; i < 19; i++)
	{
		//if (id[i] != user_card_id1[i])
		///{
		//	return false;
		//}
	}

	return true;
}

bool check_pin(char *id)
{

}


