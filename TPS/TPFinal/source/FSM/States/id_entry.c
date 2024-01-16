/*
 * id_entry.c
 *
 *  Created on: 30 ago. 2023
 *      Author: Agustin Galdeman
 */

#include "id_entry.h"

#include "Drivers/Legacy/Display.h"
#include "Drivers/HAL/Encoder.h"
#include "Drivers/Legacy/BoardLeds.h"

#include "User/userInput.h"
#include "User/userData.h"



void welcome_animation()
{
	ledOff();
	writeMessage("Insert ID", true);
}
