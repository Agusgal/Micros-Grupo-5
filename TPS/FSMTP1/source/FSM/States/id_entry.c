/*
 * id_entry.c
 *
 *  Created on: 30 ago. 2023
 *      Author: Agustin Galdeman
 */

#include "id_entry.h"

#include "../Drivers/Display.h"
#include "../Drivers/Encoder.h"

#define ID_SIZE 8



//Variables to keep track of user ID input
static uint8_t id[ID_SIZE];
static uint8_t curr_pos = 0;



void init_id (void)
{
	//give some feedback on user input (leds)
	//stop display multiplexing (animations)

}


void waiting_id()
{
	//llama funciones de drivers de display para mostrar mensaje
	//
}
