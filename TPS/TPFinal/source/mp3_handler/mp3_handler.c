/***************************************************************************/ /**
  @file     mp3_handler.c
  @brief    Handles MP3 functions like playing songs and finding metadata
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
#include "../fft/vumeter.h"

#include "fsl_common.h"
#include "EventQueue/queue.h"


static void mp3Handler_selectNextSong(void);

static void mp3Handler_selectPreviousSong(void);

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
static uint32_t sampleRate = 44100;

static MP3Object_t currObject;

static MP3Object_t playingSongFile;

static int maxObjectCount = 0;

SDK_ALIGN(static uint16_t processedAudioBuffer[BUFFER_SIZE] , SD_BUFFER_ALIGN_SIZE);
SDK_ALIGN(static short decoder_buffer[2*BUFFER_SIZE], SD_BUFFER_ALIGN_SIZE);

static uint8_t vol = 15;
static char vol2send = 15 + 40;

static float vol_ending_song = 15;
static uint8_t next_prev_song = 0; //0: nothing; 1: next_song; 2: prev_song
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

	}
}


void mp3Handler_nextObject(void)
{
	currObject = mp3Files_GetNextObject(currObject);
}


void mp3Handler_nextMP3File(void)
{
	currObject =  mp3Files_GetNextMP3File(currObject);
}


void mp3Handler_prevObject(void)
{
	currObject = mp3Files_GetPreviousObject(currObject);
}


void mp3Handler_prevMP3File(void)
{
	currObject = mp3Files_GetPreviousMP3File(currObject);
}


bool mp3Handler_selectObject(void)
{
	if (!(currObject.object_type == MP3_FILE))
	{
		if (currObject.object_type == DIRECTORY)
		{
			currObject = mp3Files_Enter_Dir(currObject);
		}
		else if (currObject.object_type == RETURN_DIR)
		{
			currObject = mp3Files_Exit_Dir(currObject);
		}
		return false;
	}

	else
	{
		// If the file is a MP3 file, load audio
		playingSongFile = currObject;

		MP3Decoder_LoadFile(playingSongFile.path);

		// First two buffers in 0V, no sound
		memset(processedAudioBuffer, DAC_ZERO_VOLT_VALUE, sizeof(processedAudioBuffer));

		sampleRate = 44100;

		// Set a default sampleRate for first buffers
		AudioPlayer_LoadSongInfo(processedAudioBuffer, sampleRate);

		mp3Handler_updateAudioPlayerBackBuffer();

		return true;

	}

}


void mp3Handler_updateAudioPlayerBackBuffer(void)
{
	uint32_t numOfSamples = 0;
	uint8_t numOfChannels = 1;
	float effects_in[BUFFER_SIZE];
	float effects_out[BUFFER_SIZE];

	AudioPlayer_UpdateBackBuffer(processedAudioBuffer, sampleRate);

	// Clean buffers to rewrite
	memset(processedAudioBuffer, 0, sizeof(processedAudioBuffer));
	memset(decoder_buffer, 0, sizeof(decoder_buffer));

	// Fetch the new frame
	decoder_result_t res = MP3Decoder_DecodeFrame(decoder_buffer, 2*BUFFER_SIZE, &numOfSamples, &sampleRate);

	// Get the number of channels in the frame
	MP3Decoder_GetLastFrameNumOfChannels(&numOfChannels);

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
			effects_in[index] = (decoder_buffer[index * 2] + decoder_buffer[index * 2 + 1]) * coef;
		}
	}

	// Apply audio effects
	EQ_Apply(effects_in, effects_out);

	// Apply volume and
	// Scale to 12 bits, to fit in the DAC
	coef = (vol * 1.0) / MAX_VOLUME;
	for (index = 0; index < BUFFER_SIZE; index++)
	{
		processedAudioBuffer[index] = (effects_out[index]*coef+1)*2048;
	}

	// Complete the rest of the buffer with the last value
	if (res == DECODER_END_OF_FILE)
	{
		uint16_t last_value = processedAudioBuffer[(numOfSamples / numOfChannels) - 1] ;
		for (uint32_t index = (numOfSamples / numOfChannels); index < BUFFER_SIZE ; index++)
		{
			processedAudioBuffer[index] = last_value;
		}

		push_Queue_Element(NEXT_SONG_EV);

	}

	// This makes a fading effect if the user wants to change song
	if(next_prev_song == 1)
	{
		vol_ending_song -= 0.4;
		coef = ((vol_ending_song) * 1.0) / MAX_VOLUME;
		for (index = 0; index < BUFFER_SIZE; index++)
		{
			processedAudioBuffer[index] = (effects_out[index]*coef+1)*2048;
		}

		if(vol_ending_song < 0)
		{
			next_prev_song = 0;
			mp3Handler_selectNextSong();
		}
	}
	else if(next_prev_song == 2)
	{
		vol_ending_song -= 0.4;
		coef = ((vol_ending_song) * 1.0) / MAX_VOLUME;
		for (index = 0; index < BUFFER_SIZE; index++)
		{
			processedAudioBuffer[index] = (effects_out[index]*coef+1)*2048;
		}

		if(vol_ending_song < 0)
		{
			next_prev_song = 0;
			mp3Handler_selectPreviousSong();
		}
	}
	else
	{
		vol_ending_song = vol;
	}

	// Compute FFT and set the vumeter
	VU_FFT(effects_out, sampleRate, 80, 10000);
}


char * mp3Handler_getCurrentName(void)
{
	char * ret;
	ret = mp3Files_GetObjectName(currObject);
	return ret;
}


void mp3Handler_deinit(void)
{
	//mp3Handler_stop();

	MP3Decoder_shutDown();

	mh_SD_disconnect();

	mp3Files_ResetObjects();

	VU_Clear_Display();

	playing = false;
	init = false;
}


void mp3Handler_showFFT(void)
{
	VU_Draw_Display();
}


void mp3Handler_updateAll(void)
{
	mp3Handler_updateAudioPlayerBackBuffer();
	mp3Handler_showFFT();
}

void mp3Handler_playNextSong(void)
{
	next_prev_song = 1;
}

static void mp3Handler_selectNextSong(void)
{
	mp3Handler_nextMP3File();

	if (currObject.object_type == MP3_FILE)
	{
		mp3Handler_selectObject();
		mp3Handler_play();
	}

}

void mp3Handler_playPreviousSong(void)
{
	next_prev_song = 2;
}

static void mp3Handler_selectPreviousSong(void)
{
	mp3Handler_prevMP3File();

	if (currObject.object_type == MP3_FILE)
	{
		mp3Handler_selectObject();
		mp3Handler_play();
	}

}


void mp3Handler_play(void)
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
	MP3Decoder_LoadFile(currObject.path);
	AudioPlayer_Pause();
	playing = false;
}


char * mp3Handler_getTitle(void)
{
	char * ret;
	if(!MP3Decoder_getFileTitle(&ret))
	{
		ret = mp3Files_GetObjectName(playingSongFile);
	}
	return ret;
}


char * mp3Handler_getArtist(void)
{
	char * ret;
	if(!MP3Decoder_getFileArtist(&ret))
	{
		ret = "-";
	}
	return ret;
}


char * mp3Handler_getAlbum(void)
{
	char * ret;
	if(!MP3Decoder_getFileAlbum(&ret))
	{
		ret = "-";
	}
	return ret;
}


char* mp3Handler_getYear(void)
{
	char * ret;
	if(!MP3Decoder_getFileYear(&ret))
	{
		ret = "-";
	}
	return ret;
}


void mp3Handler_IncVolume(void)
{
	vol += (vol >= MAX_VOLUME)? 0 : 1;
	vol2send = vol+40;
}


void mp3Handler_DecVolume(void)
{
	vol -= (vol > 0) ? 1 : 0;
	vol2send = vol + 40;
}


char mp3Handler_getVolume(void)
{
	return (char)vol;
}


void mp3Handler_setVolume(char value)
{
	if(vol <= 40 && vol >=0)
	{
		vol = (uint8_t)value;
	}
}

