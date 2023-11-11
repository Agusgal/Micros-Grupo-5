/***************************************************************************//**
  @file     ModulatorV1.h
  @brief    Modulator functions
  @author   Grupo 5
 ******************************************************************************/

#ifndef _MODULADORV1_H_
#define _MODULADORV1_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <Drivers/ModuladorV1.h>

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define STAND_LEN	11

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void Modulator_Init(void(*clb)(void));

void Modulator_sendChar(char my_char);


#endif // _MODULADORV1_H_
