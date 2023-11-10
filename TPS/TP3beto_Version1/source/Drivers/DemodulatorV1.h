/*
 * DemodulatorV1.h
 *
 *  Created on: 8 nov. 2023
 *      Author: agus
 */

#ifndef DEMODULATORV1_H_
#define DEMODULATORV1_H_



#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


#define DELAY 5 //NUMBER OF SAMPLE PERIODS
#define FIR_ORDER 20

typedef void (*myCallback)(void);

void Demodulator_Init( myCallback funcallback);
char get_Msg (void);
void demodulate(void);
bool isDataReady(void);


#endif /* DEMODULATORFSK_H_ */
#endif /* DEMODULATORV1_H_ */
