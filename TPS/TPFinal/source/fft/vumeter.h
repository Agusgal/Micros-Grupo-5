#ifndef _VU_H
#define _VU_H
#include<stdio.h>
#include<stdint.h>
#include "arm_math.h"

#define FFT_SIZE	1024

void VU_Init();
int VU_FFT(float32_t * inputSignal, float32_t sampleRate, int lowerFreqBand, int higherFreqBand);
void VU_Fill_Color_Matrix(int * vumeterMatrix);
void VU_Draw_Display();
void VU_Clear_Display();

#endif
/**
 *
 * End of file.
 */
