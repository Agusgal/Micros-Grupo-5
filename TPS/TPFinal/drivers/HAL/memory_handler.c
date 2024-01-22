/***************************************************************************/ /**
  @file     memory_manager.h
  @brief    Memory System header
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
//#include "file_system_manager.h"

#include <stdio.h>

#include "fsl_port.h"
#include "component/SD/fsl_sd.h"
#include "fsl_sd_disk.h"

#include "ff.h"
#include "diskio.h"
#include "component/SD/sdmmc_config.h"

#include "memory_manager.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
enum SD_state{IDLE = 0, CONNECTED, DISCONNECTED};

/*******************************************************************************
 * PRIVATE VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static bool SD_connected = false;
static bool SD_error = false;
static bool SD_HostInitDone = false;
static FATFS g_fileSystem;
static uint8_t SD_status = 0;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void memory_handler_insertCB(bool isInserted, void *userData);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void memory_handler_init()
{
	SYSMPU->CESR &= ~SYSMPU_CESR_VLD_MASK;

	// Initializes SD pins and detection callback
	BOARD_SD_Config(&g_sd, Mm_Callback, BOARD_SDMMC_SD_HOST_IRQ_PRIORITY, NULL);

	// Initializes Host
	if (SD_HostInit(&g_sd) != kStatus_Success)
	{
		SD_error = true;
	}

	SD_HostInitDone = true;

}



/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void memory_handler_insertCB(bool isInserted, void *userData)
{
	// If the previous state matches the actual state, there is an error
	if(SD_connected == isInserted)
	{
		SD_error = true;
	}
	// If there is no error and the SD card is connected
	else if(isInserted)
	{
		SD_status = CONNECTED;
	}
	else
	{
		SD_status = DISCONNECTED;
	}

	SD_connected = isInserted;
}
