/***************************************************************************/ /**
  @file     memory_handler.h
  @brief    Memory System header
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/


#ifndef MEMORY_HANDLER_H_
#define MEMORY_HANDLER_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
#define SD_BUFFER_ALIGN_SIZE 	(4U)

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*
 * Init routine
 */
void memory_handler_init(void);

/*
 * SD present or not
 */
bool Mm_IsSDPresent(void);

/*
 *	SD initialization routine
 */
void Mm_OnConnection(void);

/*
 * SD deinit routine
 */
void Mm_OnDesconection(void);

bool Mm_SDConnection(void);

bool Mm_SDDesconnection(void);

#endif /* MEMORY_HANDLER_H_ */
