/*
 * comunicationHandler.h
 *
 *  Created on: 8 Oct 2023
 *      Author: Grupo 5
 */

#ifndef COMUNICATIONHANDLER_H_
#define COMUNICATIONHANDLER_H_

#include <stdint.h>
#include "accel.h"

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
#define GROUPS	7

enum {ROLL_UPD = 1 , PITCHING_UPD, ORIENTATION_UPD};

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

void comunicationHandler_init(uint8_t group_num);

void comunicationHandler_send2Ext(Orient_t myBoard, uint8_t typeUPD);


void Com_EventHandler(void);

bool isWriteAvailable(uint8_t typeUPD);

void setWriteAvailable(uint8_t typeUPD, bool value);

#endif /* COMUNICATIONHANDLER_H_ */
