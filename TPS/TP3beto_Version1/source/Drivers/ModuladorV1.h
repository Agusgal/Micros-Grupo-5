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


/**
 * @brief Inicializa el modulador
 * @param clb 	funcion de callback que se ejecuta una vex terminada la modulacion (puede servir para debugging por ejemplo)
 * @return
 */
void Modulator_Init(void(*clb)(void));


/**
 * @brief
 * @param uint8_t data
 * @return 	int8_t	-1 if max limit is exceeded
 */
int8_t modulator_sendChar(uint8_t data);


#endif // _MODULADORV1_H_
