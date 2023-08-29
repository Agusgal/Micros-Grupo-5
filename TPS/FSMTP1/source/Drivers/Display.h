/***************************************************************************//**
  @file     gpio.h
  @brief    Simple GPIO Pin services, similar to Arduino
  @author   Nicolás Magliola
 ******************************************************************************/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

void Display_Init(void);
void updateDisplay(char txt []);
void writeDigit (int number,uint8_t digit);
void writeMessage(char * message, bool scroll);
void pauseMessage();
void pauseScroll();
void continueScroll(char scroll);
void ScrollRightOnce();
void ScrollLeftOnce();
void toggleScroll();
void muxDisplay();


char * int2str (int num);
/*******************************************************************************
 ******************************************************************************/

#endif // _GPIO_H_
