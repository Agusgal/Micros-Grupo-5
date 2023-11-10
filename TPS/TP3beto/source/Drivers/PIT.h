/*
 * PIT.h
 *
 *  Created on: 5 nov. 2023
 *      Author: agus
 */



#ifndef PIT_H_
#define PIT_H_

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/**
 * @brief Initializes PIT module
 * @param
 * @return
 */
void PIT_init(void);


/**
 * @brief setea valor del timer de PIT.
 * @param valor 	de tiempo entre intervalos de PIT. IMPORTANTE, se debe ingresar el tiempo dividido 20 ns.
 * @param clb 		callback a ejecutarse junto al timer.
 * @return
 */
void PIT_set_Timer(uint8_t id, uint32_t value, void(*clb)(void));


/**
 * @brief Comienza conteo de timer
 * @param id que timer se quiere iniciar.
 * @return
 */
void PIT_Start_Timer(uint8_t timer_id);

/**
 * @brief Para el temporizador.
 * @param id     que timer se quiere parar.
 * @return
 */
void PIT_Stop_Timer(uint8_t timer_id);



#endif /* PIT_H_ */



