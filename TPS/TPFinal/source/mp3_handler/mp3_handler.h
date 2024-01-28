/***************************************************************************/ /**
  @file     mp3_handler.c
  @brief
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
 * @brief Loads previous
 */
void mp3Handler_prevObject(void);

/**
 * @brief Selects the current object
 * @return bool. True if the object selected is a MP3 File. False otherwise
 */
bool mp3Handler_selectObject(void);

/**
 * @brief Selects the current object
 *
 */
void mp3Handler_updateBuffer(void);

void mp3Handler_deinit(void);

void mp3Handler_playPrevFile(void);

void mp3Handler_playNextFile(void);

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
