/***************************************************************************/ /**
  @file     mp3_handler.c
  @brief
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

/******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <mp3_decoder.h>
#include <stdint.h>

#include "mp3_handler.h"
#include "../mp3_file_handler/mp3_file_handler.h"
#include "memory_handler.h"
#include "AudioPlayer.h"
#include "equalizer.h"
#include "../drivers/HAL/mp3_decoder.h"

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

static MP3Object_t currObject;

static MP3Object_t playingSongFile;

static int maxObjectCount = 0;

SDK_ALIGN(static uint16_t g_bufferRead[BUFFER_SIZE] , SD_BUFFER_ALIGN_SIZE);
SDK_ALIGN(static short decoder_buffer[2*BUFFER_SIZE], SD_BUFFER_ALIGN_SIZE);
static float effects_in[BUFFER_SIZE], effects_out[BUFFER_SIZE];

static uint8_t vol = 15;
static char vol2send = 15 + 40;
/******************************************************************************

 ******************************************************************************/
void mp3Handler_init(void)
{
	if(!init)
	{
		// Mount SD
		mh_SD_mount();

		// Generates the main directory structure
		mp3Files_Init();

		// Search for the first object
		currObject = mp3Files_GetFirstObject();

		// Search for the number of objects in the main directory
		maxObjectCount = mp3Files_GetObjectsCounter();

		init = !init;
	}
}


void mp3Handler_nextObject(void)
{
	currObject = mp3Files_GetNextObject(currObject);
}


void mp3Handler_prevObject(void)
{
	currObject = mp3Files_GetPreviousObject(currObject);
}


bool mp3Handler_selectObject(void)
{
	if (!(currObject.object_type == MP3_FILE))
	{
		if (currObject.object_type == DIRECTORY)
		{
			 mp3Files_Enter_Dir(currObject);
		}
		else if (currObject.object_type == RETURN_DIR)
		{
			mp3Files_Exit_Dir(currObject);
		}
		return false;
	}

	else
	{
		// If the file is a MP3 file, load audio

		playingSongFile = currObject;

		MP3Decoder_LoadFile(playingSongFile.path);

		// For testing, decode the frame
		uint8_t decodedDataBuffer[512];
		uint32_t* numSamplesDecoded = 0;

		MP3Decoder_DecodeFrame(decodedDataBuffer, 512, numSamplesDecoded);

		int i = 1;
		/* Primeros dos buffer constante, no hay sonido */
		//memset(g_bufferRead, 0x08, sizeof(g_bufferRead));

		/* Podria buscar el sample rate y mandarlo */
		//AudioPlayer_LoadSongInfo(g_bufferRead, 44100);

		//Audio_updateBuffer();

	}

}
