/***************************************************************************/ /**
  @file     mp3_handler.c
  @brief	Handles MP3 functions like playing songs and finding metadata
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

#ifndef _MP3_HANDLER_H_
#define _MP3_HANDLER_H_

#include <stdbool.h>

/**
 * @brief Initializes the mp3 Handler
 */
void mp3Handler_init(void);


/**
 * @brief Loads next object
 */
void mp3Handler_nextObject(void);


/**
 * @brief Loads next object
 */
void mp3Handler_nextMP3File(void);


/**
 * @brief Loads previous
 */
void mp3Handler_prevObject(void);


/**
 * @brief Loads previous
 */
void mp3Handler_prevMP3File(void);


/**
 * @brief Selects the current object
 * @return bool. True if the object selected is a MP3 File. False otherwise
 */
bool mp3Handler_selectObject(void);


/**
 * @brief Update AudioPlayer Back buffer
 *
 */
void mp3Handler_updateAudioPlayerBackBuffer(void);


/**
 *  @brief Play next_song
 */
void mp3Handler_playNextSong(void);


/**
 *  @brief Play previous song
 */
void mp3Handler_playPreviousSong(void);

/**
 *  @brief Play previous song
 */

void mp3Handler_deinit(void);


char * mp3Handler_getCurrentName(void);


char * mp3Handler_getArtist(void);


char * mp3Handler_getAlbum(void);


char * mp3Handler_getYear(void);


void mp3Handler_updateAll(void);


void mp3Handler_showFFT(void);

void mp3Handler_play(void);


void mp3Handler_toggle(void);


void mp3Handler_stop(void);


void mp3Handler_IncVolume(void);


void mp3Handler_DecVolume(void);


char mp3Handler_getVolume(void);


void mp3Handler_setVolume(char value);


#endif /* _MP3_HANDLER_H_ */
