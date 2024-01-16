/*******************************************************************************
  @file     ftm.h

  @brief    FTM Driver

  @author 	Grupo 5
 ******************************************************************************/

#ifndef _FTM_H_
#define _FTM_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define NULL 0
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

// FlexTimer Channels
typedef enum {
	FTM_CH_0,
	FTM_CH_1,
	FTM_CH_2,
	FTM_CH_3,
	FTM_CH_4,
	FTM_CH_5,
	FTM_CH_6,
	FTM_CH_7,
	FTM_CH_COUNT
} FTM_Channel_t;

// FlexTimer Modules
typedef enum {
	FTM_0,
	FTM_1,
	FTM_2,
	FTM_3,
	FTM_COUNT
} FTM_Module_t;

// Input Capture Modes
typedef enum {
	FTM_IC_RISING_EDGE,
	FTM_IC_FALLING_EDGE,
	FTM_IC_BOTH_EDGES
} FTM_IC_Mode_t;

// Output Compare Modes
typedef enum {
	FTM_OC_TOGGLE,
	FTM_OC_CLEAR,
	FTM_OC_SET
} FTM_OC_Mode_t;

// PWM Modes
typedef enum {
	FTM_PWM_HIGH_PULSES,
	FTM_PWM_LOW_PULSES
} FTM_PWM_Mode_t;

// PWM Alignments
typedef enum {
	FTM_PWM_EDGE_ALIGNED,
	FTM_PWM_CENTER_ALIGNED
} FTM_PWM_Align_t;

typedef enum
{
	FTM_PSC_x1		= 0x00,
	FTM_PSC_x2		= 0x01,
	FTM_PSC_x4		= 0x02,
	FTM_PSC_x8		= 0x03,
	FTM_PSC_x16		= 0x04,
	FTM_PSC_x32		= 0x05,
	FTM_PSC_x64		= 0x06,
	FTM_PSC_x128	= 0x07,

} FTM_PS_t;

typedef void 	(*FTM_Callback_t)	(void);
typedef void	(*CH_Callback_t)	(uint16_t);

/*******************************************************************************

 	 	 	 	    FUNCTION PROTOTYPES WITH GLOBAL SCOPE

******************************************************************************/

/***************************************************
 * 				FTM Services
 **************************************************/

/*
 * @brief Init FTM0 with modulo 0xFFFF
 * @param prescaler		Frequency divider
 * @param callback		Overflow Callback
 */
void FTM0_Init(FTM_PS_t prescaler, FTM_Callback_t callback);


void FTM_Init(FTM_Module_t module, FTM_PS_t prescaler, uint16_t modulo, FTM_Callback_t callback);


void FTM_Start(FTM_Module_t module);


void FTM_Stop(FTM_Module_t module);


void FTM_Restart(FTM_Module_t module);


uint16_t FTM_GetCount(FTM_Module_t module);


/***************************************************
 * 				FTM Channel Services
 **************************************************/

void FTM_CH_SetCount(FTM_Module_t module, FTM_Channel_t channel, uint16_t count);


uint16_t FTM_CH_GetCount(FTM_Module_t module, FTM_Channel_t channel);


void FTM_CH_AddCallback(FTM_Module_t module, FTM_Channel_t channel, CH_Callback_t callback);


void FTM_CH_EnableDMA(FTM_Module_t module, FTM_Channel_t channel);


uint32_t * FTM_CH_GetCnVPointer(FTM_Module_t module, FTM_Channel_t channel);

/***************************************************
 * 				Input Capture Services
 **************************************************/

void FTM_CH_IC_Init(FTM_Module_t module, FTM_Channel_t channel, FTM_IC_Mode_t mode,CH_Callback_t callback);

/***************************************************
 * 				Output Compare Services
 **************************************************/

void FTM_CH_OC_Init(FTM_Module_t module, FTM_Channel_t channel, FTM_OC_Mode_t mode, bool outInit,CH_Callback_t callback);


void FTM_CH_OC_Start(FTM_Module_t module, FTM_Channel_t channel, uint16_t count);


void FTM_CH_OC_Stop(FTM_Module_t module, FTM_Channel_t channel);

/***************************************************
 * 				PWM Services
 **************************************************/

void FTM_CH_PWM_Init(FTM_Module_t module, FTM_Channel_t channel, FTM_PWM_Mode_t mode, FTM_PWM_Align_t alignment, uint16_t duty, uint16_t period,CH_Callback_t callback);


void FTM_PWM_SetDuty(FTM_Module_t module, FTM_Channel_t channel, uint16_t duty);


void FTM_PWM_ON(FTM_Module_t module, FTM_Channel_t channel);


void FTM_PWM_OFF(FTM_Module_t module, FTM_Channel_t channel);

/*******************************************************************************
 ******************************************************************************/

#endif
