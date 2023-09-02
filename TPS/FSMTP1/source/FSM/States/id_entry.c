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

#include "User/userInput.h"
#include "User/userData.h"



void welcome_animation()
{
	writeMessage("Insert ID", true);
}
