/***************************************************************************//**
  @file     vumeter.h
  @brief    Vumeter functions
  @author   Grupo 5
 ******************************************************************************/


#ifndef _VU_H
#define _VU_H

#include<stdio.h>
#include<stdint.h>
#include "arm_math.h"



#define FFT_SIZE	1024


/*!
 * @brief Initializes vumeter
 * @param void.
 */
void VU_Init();


/*!
 * @brief Calculates FFT of inputsignal and writes matrix display with updated info
 *
 * @param inputSignal: signal array passed by reference, array with data to calculate FFT
 * @param sampleRate: sample rate of signal/fft
 * @param lowerFreqBand:
 * @param higherFreqBand:
 *
 * @return 0 if everything went correctly
 */
int VU_FFT(float32_t * inputSignal, float32_t sampleRate, int lowerFreqBand, int higherFreqBand);


/*!
 * @brief draws the content of Color_Matrix into the matrix display (using matrix driver)
 *
 * @param void
 *
 * @return void
 */
void VU_Draw_Display(void);


/*!
 * @brief Initializes vumeter
 * @param void.
 */
void VU_Clear_Display();

#endif
