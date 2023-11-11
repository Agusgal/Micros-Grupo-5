/***************************************************************************//**
  @file     modulador.h
  @brief
  @author   Grupo 5
 ******************************************************************************/

#ifndef _MODULADOR_H_
#define _MODULADOR_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>


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
/**
 * @brief
 */
void modulador_init(void);

/**
 * @brief
 * @return 	int8_t	-1 if max limit is exceeded
 */
int8_t modulador_send_char(uint8_t data);

/*******************************************************************************
 ******************************************************************************/

#endif // _MODULADOR_H_