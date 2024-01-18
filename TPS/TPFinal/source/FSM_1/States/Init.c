/***************************************************************************/ /**
  @file     init_state.c
  @brief    Init state functions.
  @author   Grupo 5 - Lab de Micros
 ******************************************************************************/

#include "Init.h"
#include "idle.h"


#include "queue.h"
#include "memory_manager.h"
#include "power_mode_switch.h"

void startProgram(void)
{
	//Some SD card shenanigans
	/*
	if(Mm_SDConnection())
	{
		// Fetch the sd on conection event
		push_Queue_Element(SD_IN_EV);
	}
	else if(Mm_IsSDPresent())
	{
		//The sd is present but the event has been fetch while not able to handle
		push_Queue_Element(SD_IN_EV);
	}*/

	// Start the idle state, not best practice to do here ( ͡° ͜ʖ ͡°)
	Idle_InitState();
}
