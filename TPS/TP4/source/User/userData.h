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

int getSelectedUSer(void);

int getFloor(int user);

void init_data_base(void);

bool isInside(int user);

void enterBuilding(int user);

void exitBuilding(int user);

typedef struct User
{
	char *id;
	char *pin;
	int floor;
	bool inBuilding;
	uint8_t card_id[19];
}User;

#endif /* USERDATA_H_ */
