/***************************************************************************/ /**
  @file     mp3_handler.c
  @brief
  @author   Grupo 5 - Lab de Micros
 ******************************************************************************/

/******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>

#include "mp3_handler.h"
#include "file_system_manager.h"
#include "memory_handler.h"
#include "AudioPlayer.h"
#include "vumeterRefresh.h"
#include "decoder.h"
#include "equalizer.h"

#include "fsl_common.h"
#include "EventQueue/queue.h"


/******************************************************************************
 * DEFINES
 ******************************************************************************/

#define BUFFER_SIZE (AUDIO_PLAYER_BUFF_SIZE)
#define MAX_VOLUME	(40U)

/*******************************************************************************
 * LOCAL VARIABLES
 ******************************************************************************/
static bool playing = false;
static bool init = false;

static Mp3File_t currFile;
static Mp3File_t playingFile;

static int maxFile = 0;

SDK_ALIGN(static uint16_t g_bufferRead[BUFFER_SIZE] , SD_BUFFER_ALIGN_SIZE);
SDK_ALIGN(static short decoder_buffer[2*BUFFER_SIZE], SD_BUFFER_ALIGN_SIZE);
static float effects_in[BUFFER_SIZE], effects_out[BUFFER_SIZE];

static uint8_t vol = 15;
static char vol2send = 15 + 40;
/******************************************************************************

 ******************************************************************************/
void Audio_init(void)
{
	if(!init)
	{
		Mm_OnConnection(); //Init the SD;
		FileSystem_ScanFiles(); // Build file system tree
		currFile = FileSystem_GetFirstFile();
		maxFile = FileSystem_GetFilesCount();
		init = !init;
	}
}

