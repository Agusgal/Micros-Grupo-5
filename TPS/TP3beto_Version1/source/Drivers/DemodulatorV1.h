/*
 * demodulatorFSK.h
 *
 *  Created on: Nov. 2023
 *      Author: grupo 5
 */

#ifndef DEMODULARDORV1_H_
#define DEMODULARDORV1_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


#define DELAY 5 //NUMBER OF SAMPLE PERIODS
#define FIR_ORDER 18



typedef void (*myCallback)(void);


/**
 * @brief Initializes PIT module
 * @param
 * @return
 */
void Demodulator_Init(myCallback clb);


/**
 * @brief Initializes PIT module
 * @param
 * @return
 */
char get_Msg (void);


/**
 * @brief Demodula la señal, toma la señal del salida del ADC sample a sample, la multiplica y la filtra con el FIR.
 * @return
 */
void demodulate(void);


/**
 * @brief Initializes PIT module
 * @param
 * @return
 */
bool isDataReady(void);





#endif /* DEMODULARDORV1_H_ */
