/***************************************************************************/ /**
  @file     mp3_handler.c
  @brief
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

#ifndef MP3_HANDLER_H_
#define MP3_HANDLER_H_

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

void Audio_deinit(void);


void Audio_playPrevFile(void);

void Audio_playNextFile(void);

char * Audio_getCurrentName(void);

char * Audio_getArtist(void);

char * Audio_getAlbum(void);

char * Audio_getYear(void);

void Audio_updateAll(void);

void Audio_updateBuffer(void);

void Audio_showFFT(void);

void Audio_play(void);

void Audio_toggle(void);

void Audio_stop(void);


void Audio_IncVolume(void);

void Audio_DecVolume(void);

char Audio_getVolume(void);

void Audio_setVolume(char value);

#endif /* MP3_HANDLER_H_ */
