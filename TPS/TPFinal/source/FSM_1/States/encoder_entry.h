/*
 * id_entry.h
 *
 *  Created on: 30 ago. 2023
 *      Author: Agustin Galdeman
 */

#ifndef ENCODER_ENTRY_H
#define ENCODER_ENTRY_H


#include <stdint.h>


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Initializes user input
 */


void init_id(void);

void up_number(void);
void down_number(void);
void accept_number(void);
//void msg_fail_encoder(void);
//void msg_ok_encoder(void);

#endif /* ENCODER_ENTRY_H */
