/*
 * ADC.c
 *
 *  Created on: 7 nov. 2023
 *      Author: agus
 */


#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define ADC0_DP0	0b00000
#define DISABLE		0b11111

typedef uint16_t ADCData_t;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


/**
 * @brief ADC Inicialization
 * @param adc_channel      canal a utilizar
 * @param clb			   callback a ejecutar
 * @return
 */
void ADC_Init(uint32_t adc_ch, void (*clb)(void));


/**
 * @brief Activacion de ADC
 * @param adc_channel 	canal  a activar
 * @return
 */
void ADC_EnableModule(uint32_t adc_ch);


/**
 * @brief Desactiva el ADC
 * @return
 */
void ADC_DisableModule(void);


/**
 * @brief getter de data del ADC
 * @return
 */
ADCData_t ADC_getData(void);


/**
 * @brief Inicia conversion de datos
 * @return
 */
void ADC_StartConvertion(void);



#endif /* ADC_H_ */
