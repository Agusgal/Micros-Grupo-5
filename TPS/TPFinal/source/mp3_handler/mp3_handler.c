/***************************************************************************/ /**
  @file     mp3_handler.c
  @brief
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

/******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
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

SDK_ALIGN(static uint16_t processedAudioBuffer[BUFFER_SIZE] , SD_BUFFER_ALIGN_SIZE);
SDK_ALIGN(static short decoder_buffer[2*BUFFER_SIZE], SD_BUFFER_ALIGN_SIZE);

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

		// Primeros dos buffer constante, no hay sonido */
		memset(processedAudioBuffer, 0x08, sizeof(processedAudioBuffer));

		AudioPlayer_LoadSongInfo(processedAudioBuffer, 44100);

		mp3Handler_updateBuffer();

	}

}


void mp3Handler_updateBuffer(void)
{
	uint16_t numOfSamples = 0;
	uint8_t numOfChannels = 1;
	float effects_in[BUFFER_SIZE];
	float effects_out[BUFFER_SIZE];

	AudioPlayer_UpdateBackBuffer(processedAudioBuffer);

	// Clean buffers to rewrite
	memset(processedAudioBuffer, 0, sizeof(processedAudioBuffer));
	memset(decoder_buffer, 0, sizeof(decoder_buffer));

	// Fetch the new frame
	decoder_return_t res = MP3Decoder_DecodeFrame(decoder_buffer, 2*BUFFER_SIZE, &numOfSamples, &sampleRate);

	// Get the number of channels in the frame
	MP3Decoder_GetLastFramenumOfChannels(&numOfChannels);

	// Scale from int16 to float[-1;1]
	float coef = 1.0/32768.0;
	uint32_t index;

	if(numOfChannels == 1)
	{
		for (index = 0; index < BUFFER_SIZE; index++)
		{
			effects_in[index] = decoder_buffer[index] * coef;
		}
	}
	else
	{
		// If stereo, sum L + R
		for (index = 0; index < BUFFER_SIZE; index++)
		{
			effects_in[index] = (decoder_buffer[index] + decoder_buffer[index + 1]) * coef;
		}
	}

	// TODO: Juampi effects
	equalizer_equalize(effects_in, effects_out);

	// Apply volume and
	// Scale to 12 bits, to fit in the DAC
	coef = (vol * 1.0) / MAX_VOLUME;
	for (index = 0; index < BUFFER_SIZE; index++)
	{
		processedAudioBuffer[index] = (effects_out[index]*coef+1)*2048;
	}

	// Complete the rest of the buffer with 0V
	if (res == DECODER_END_OF_FILE)
	{
		for (uint32_t index = (numOfSamples / numOfChannels); index < BUFFER_SIZE ; index++)
		{
			processedAudioBuffer[index] = 2048;
		}

		push_Queue_Element(NEXT_SONG_EV);

	}

	VU_FFT(effects_out, sampleRate, 80, 10000);
}


void mp3Handler_showFFT(void)
{
	vumeterRefresh_draw_display();
}


void mp3Handler_updateAll(void)
{
	mp3Handler_updateBuffer();
	mp3Handler_showFFT();


mp3Handler_void mp3Handler_play(void)
{
	AudioPlayer_Play();
	playing = true;
}


void mp3Handler_toggle(void)
{
	if(playing)
		AudioPlayer_Pause();
	else
		AudioPlayer_Play();

	playing = !playing;
}


void mp3Handler_stop(void)
{
	decoder_MP3LoadFile(currFile.path);
	AudioPlayer_Pause();
	playing = false;
}


char * mp3Handler_getName(void)
{
	char * ret;
	if(!decoder_getFileTitle(&ret))
	{
		ret = FileSystem_GetFileName(playingFile);
	}
	return ret;
}


char * mp3Handler_getArtist(void)
{
	char * ret;
	if(!decoder_getFileArtist(&ret))
	{
		ret = "-";
	}
	return ret;
}


char * mp3Handler_getAlbum(void)
{
	char * ret;
	if(!decoder_getFileAlbum(&ret))
	{
		ret = "-";
	}
	return ret;
}


char mp3Handler_o_getYear(void)
{
	char * ret;
	if(!decoder_getFileYear(&ret))
	{
		ret = "-";
	}
	return ret;
}


void mp3Handler_IncVolume(void)
{
	vol += (vol >= MAX_VOLUME)? 0 : 1;
	vol2send = vol+40;
	esp_Send(2, &vol2send, 1);
}


void mp3Handler_DecVolume(void)
{
	vol -= (vol > 0) ? 1 : 0;
	vol2send = vol+40;
	esp_Send(2, &vol2send, 1);
}


char mp3Handler_getVolume(void)
{
	return (char)vol;
}


void mp3Handler_setVolume(char value)
{
	if(vol <= 40 && vol >=0)
	{
		vol = value;
	}
}
