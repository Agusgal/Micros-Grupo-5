/*
 * DAC.h
 *
 *  Created on: 6 nov. 2023
 *      Author: agus
 */

#ifndef DAC_H_
#define DAC_H_

#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

typedef uint16_t DACDATA_t;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/**
 * @brief Inicializa modulo de DAC
 * @return
 */
void DAC_init(void);



/**
 * @brief Setter data input DAC
 * @return
 */
void DAC_setData(DACDATA_t data);


#endif /* DAC_H_ */
