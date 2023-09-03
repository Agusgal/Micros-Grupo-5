/*
 * brightness.c
 *
 *  Created on: 3 sep. 2023
 *      Author: agus
 */

#include "brightness.h"
#include "Drivers/Display.h"



void incr_bri(void)
{
	incBrightness();
}

void decr_bri(void)
{
	decBrightness();
}
