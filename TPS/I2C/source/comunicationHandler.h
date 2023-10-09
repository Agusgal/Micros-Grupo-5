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
#define GROUPS	6

enum {ROLL_UPD = 1 , PITCHING_UPD, ORIENTATION_UPD};


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

void comunicationHandler_init(uint8_t group_num);

void comunicationHandler_send2Ext(Orient_t myBoard, uint8_t typeUPD);

int16_t testParser(char* buf, int16_t* boardDATA, uint8_t typeUPD, Orient_t myBoard);

void Com_EventHandler(void);


#endif /* COMUNICATIONHANDLER_H_ */
