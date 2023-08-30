/***************************************************************************//**
  @file     gpio.h
  @brief    Simple GPIO Pin services, similar to Arduino
  @author   Nicolás Magliola
 ******************************************************************************/

<<<<<<<< HEAD:TPS/FSMTP1/source/Drivers/Display.h
#ifndef _DISPLAY_H_
#define _DISPLAY_H_
========
#ifndef _ENCODER_H_
#define _ENCODER_H_
>>>>>>>> tests-jp:TPS/FSMTP1/source/Drivers/Encoder.h

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

<<<<<<<< HEAD:TPS/FSMTP1/source/Drivers/Display.h

========
#define IDLE 0
#define RISING_FLANK 2
>>>>>>>> tests-jp:TPS/FSMTP1/source/Drivers/Encoder.h
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

<<<<<<<< HEAD:TPS/FSMTP1/source/Drivers/Display.h
========
enum encoderStates {ACW1=1,ACW2,ACW3,ACW4,CW1,CW2,CW3,CW4};
enum events {ANTI_CLOCKWISE_TURN=1, CLOCKWISE_TURN,ENC_SW_PRESS};


>>>>>>>> tests-jp:TPS/FSMTP1/source/Drivers/Encoder.h

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

<<<<<<<< HEAD:TPS/FSMTP1/source/Drivers/Display.h
void Display_Init(void);
void updateDisplay(char txt []);
void writeDigit (int number,uint8_t digit);

========
void Encoder_Init(void);
int EncoderStatus(void);
int EncoderSwitchRead(void);

// NO SIRVEN, SOLO PARA TEST
>>>>>>>> tests-jp:TPS/FSMTP1/source/Drivers/Encoder.h
/*******************************************************************************
 ******************************************************************************/

#endif // _GPIO_H_
