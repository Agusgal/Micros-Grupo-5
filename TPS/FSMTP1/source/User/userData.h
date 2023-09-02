/*
 * userData.h
 *
 *  Created on: 30 ago. 2023
 *      Author: Agustin Galdeman,
 */

#ifndef USERDATA_H_
#define USERDATA_H_

#define ID_SIZE_USER 8

#include <stdbool.h>
#include <stdint.h>


bool check_encoder_id(char *id);
bool check_card_id (uint8_t *id);
bool check_pin(char *pin);



#endif /* USERDATA_H_ */
