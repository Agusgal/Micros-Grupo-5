/***************************************************************************/ /**
  @file     idle_state.c
  @brief    Idle state functions.
  @author   Grupo 5 - Lab de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "idle.h"
#include "../../EventQueue/queue.h"

#include "power_mode_switch.h"

#include "OLEDdisplay.h"

#include "Systick.h"
#include "daytime.h"
#include "Timer.h"

#include "vumeter.h"

#include "../../drivers/HAl/memory_handler.h"
#include "../mp3_handler/mp3_handler.h"




/*********************************************************
 * 		LOCAL STRUCTS AND ENUMS
 ********************************************************/
typedef enum
{
  LOW_CONSUMPTION,
  HIGH_CONSUMPTION,
} EnergyConsumptionMode_t;



/*******************************************************************************
 * PRIVATE VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
int timeCallbackId = -1;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
/**
 * @brief Shows the current time on the display.
 */
//static void showTime(void);

/**
 * @brief Changes the energy consumption mode of the device.
 * @param EnergyConsumptionMode_t energy mode.
 */
static void setEnergyConsumptionMode(EnergyConsumptionMode_t energyConsumptionMode);


/*
 *@brief Callback after init idle to change to low power mode
 */
static void changePowerMode(void);

/*
 *@brief Callback after changing to high power mode
 */
static void emitStartEv(void);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void Idle_InitState(void)
{
	//DeInit some modules for Initialization
	mp3Handler_deinit();

	timeCallbackId = Timer_AddCallback(changePowerMode, 1000, true); //Delay until related stuff is finished
}

void Idle_OnUserInteraction(void)
{
	//Memory stuff
	if (!mh_is_SD_connected())
	{
		return;
	}

	setEnergyConsumptionMode(HIGH_CONSUMPTION);

	if(timeCallbackId != -1)
	{
		Timer_Delete(timeCallbackId);
		timeCallbackId = -1;
	}

	//todo: si lo llegamos a hacer...
	//Daytime_Disable();


	timeCallbackId = Timer_AddCallback(emitStartEv, 3000, true); //Delay until clock stabilizes

}

//todo: Se va a usar en app cuando este implementado daytime
/*
void Idle_UpdateTimeClb()
{
	TimeServiceDate_t date = TimeService_GetCurrentDateTime();

	char dateString[16];
	char timeString[16];
	snprintf(dateString, sizeof(dateString), "   %02hd-%02hd-%04hd     ", date.day, date.month, date.year);
	snprintf(timeString, sizeof(timeString), "    %02hd:%02hd:%02hd      ", date.hour,date.minute, date.second);


	LCD_writeStrInPos(timeString, 15, 0, 0);
	LCD_writeStrInPos(dateString, 15, 1, 0);
}
*/

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void changePowerMode(void)
{
	setEnergyConsumptionMode(LOW_CONSUMPTION);
	timeCallbackId = -1;

	//todo: creo que el oled no pint nada aca
	//OLED_UpdateClock();

	//todo: si hacemos dia/hora hay que agregar esto
	//DayTime_Enable();

	SysTick_UpdateClk();
}


static void emitStartEv(void)
{
	timeCallbackId = -1;

	//todo: no se si va esto xd
	//OLED_UpdateClock();

	SysTick_UpdateClk();
	push_Queue_Element(START_EV);
}


static void setEnergyConsumptionMode(EnergyConsumptionMode_t energyConsumptionMode)
{
	  switch (energyConsumptionMode)
	  {
	  case LOW_CONSUMPTION:
		  PowerMode_SetVLPRMode();
		break;

	  case HIGH_CONSUMPTION:
		  PowerMode_SetRunMode();
		break;

	  default:
		break;
	  }
}
