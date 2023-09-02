/*
 * userInput.h
 *
 *  Created on: 31 ago. 2023
 *      Author: agus
 */

#ifndef USER_USERINPUT_H_
#define USER_USERINPUT_H_

#include <stdint.h>
#include <stdbool.h>



//order: 0 1 2 3 4 5 6 7 8 9 BACKSPACE

#define EMPTY_CHAR '-'
#define BACKSPACE_R '*'
#define BACKSPACE_L '*'


void reset_array(char *input_array, uint8_t *curr_pos, int array_size);


void decrease_number(char *input_array, char curr_pos);

void increase_number(char *input_array, char curr_pos);

void input_number(char *input_array, uint8_t *curr_pos, int array_size);

int get_used_entries(char *input_array, int array_size);

#endif /* USER_USERINPUT_H_ */
