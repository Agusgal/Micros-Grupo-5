/*
 * userInput.c
 *
 *  Created on: 31 ago. 2023
 *      Author: agus
 */

#include "userInput.h"
#include <stdbool.h>


static void delete_last_entry(char *input_array, int array_size);



void reset_array(char *input_array, char *curr_pos, int array_size)
{
	for(int i = 0; i < array_size; i++)
	{
		input_array[i] = EMPTY_CHAR;
	}

	*curr_pos = 0;
}


void decrease_number(char *input_array, char curr_pos)
{
	if (input_array[curr_pos] > '0' && input_array[curr_pos] <= '9')
	{
		input_array[curr_pos]--;
	}
	else if (input_array[curr_pos] == EMPTY_CHAR)
	{
		input_array[curr_pos] = '0';
	}
	else if (input_array[curr_pos] == BACKSPACE_L)
	{
		input_array[curr_pos] = '9';
	}
	else
	{
		input_array[curr_pos] = BACKSPACE_L;
	}
}

void increase_number(char *input_array, char curr_pos)
{
	if (input_array[curr_pos] == BACKSPACE_R)
	{
		input_array[curr_pos] = '0';
	}
	else if (input_array[curr_pos] == EMPTY_CHAR)
	{
		input_array[curr_pos] = '0';
	}
	else if (input_array[curr_pos] < '9')
	{
		input_array[curr_pos]++;
	}
	else
	{
		input_array[curr_pos] = BACKSPACE_R;
	}
}

void input_number(char *input_array, char *curr_pos, int array_size)
{
	if(input_array[*curr_pos] == BACKSPACE_L || input_array[*curr_pos] == BACKSPACE_R)
	{
		delete_last_entry(input_array, array_size);
		*(curr_pos)--;
	}
	else if ((input_array[*curr_pos] >= '0' && input_array[*curr_pos] <= '9') && (curr_pos != array_size - 1))
	{
		*(curr_pos)++;
		input_array[*curr_pos] = EMPTY_CHAR;
	}
}


int get_used_entries(char *input_array, int array_size)
{
	int length = 0;
	bool found = false;

	while (!found && length < array_size)
	{
		int8_t last_char = input_array[length];
		if (last_char == BACKSPACE_L || last_char == BACKSPACE_R || last_char == EMPTY_CHAR)
		{
			found = true;
		}
		else
		{
			length++;
		}
	}
	return length;
}




static void delete_last_entry(char *input_array, int array_size)
{
	int used_size = get_used_entries(input_array, array_size);

	if (used_size == 0)
	{
		//deberia volver a otro estado?????
	}
	else
	{
		input_array[used_size - 1] = EMPTY_CHAR;
		//write on display //TODO: DONDE ESCRIBO>?????
	}
}

