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

// FlexTimer Module
typedef enum {
	FTM_0,
	FTM_1,
	FTM_2,
	FTM_3,
	FTM_MOD_COUNT
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
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*****************************
*                            *
* FTM TIMER GENERAL SERVICES *
*                            *
*****************************/

void FTM0_Init(FTM_PS_t prescaler, FTM_Callback_t callback);
/*
 * @brief Configures the FlexTimer module or instance selected, the timer remains stopped.
 * @param instance 		FTM Instance
 * @param prescaler		Frequency divider
 * @param module		Value to reset the count
 * @param callback		Overflow callback (not individual channel)
 */
void FTM_Init(FTM_Module_t module, FTM_PS_t prescaler, uint16_t modulo, FTM_Callback_t callback);

/*
 * @brief Starts running the FlexTimer module.
 * @param instance		FTM Instance
 */
void FTM_Start(FTM_Module_t instance);

/*
 * @brief Stops running the FlexTimer module.
 * @param instance		FTM Instance
 */
void FTM_Stop(FTM_Module_t instance);

void FTM_Restart(FTM_Module_t module);

/*
 * @brief Returns the current value of the FlexTimer counter.
 * @param instance		FTM Instance
 */
uint16_t FTM_GetCount(FTM_Module_t instance);





void FTM0_OVF_Callback (void);
/*****************************
*                            *
*  CHANNEL GENERAL SERVICES  *
*                            *
*****************************/

/**
 * @brief 	Gets CnV pointer, for DMA porpuses
 */
uint32_t* FTM_CH_GetCnVPointer(FTM_Module_t module, FTM_Channel_t channel);

/*
 * @brief Sets the value of the channel's count register. Particularly useful when running
 * 	      channel as output compare mode to set the value where the match event is triggered.
 * @param instance		FTM Instance
 * @param channel		FTM Channel
 * @param count			Sets the value of the channel count where matching process occurs
 */
void FTM_CH_SetCount(FTM_Module_t instance, FTM_Channel_t channel, uint16_t count);
void FTM_CH_EnableDMA(FTM_Module_t instance, FTM_Channel_t channel);
/*
 * @brief Returns current value of the channel count register.
 * @param instance		FTM Instance
 * @param channel		FTM Channel
 */
uint16_t FTM_CH_GetCount(FTM_Module_t instance, FTM_Channel_t channel);

/**
 * @param callback		Channel callback
 */
void FTM_CH_AddCallback(FTM_Module_t module, FTM_Channel_t channel, CH_Callback_t callback);

void FTM_CH_Init(FTM_Module_t module, FTM_Channel_t channel, uint16_t count, CH_Callback_t callback);

/*************************************
*                                    *
*   INPUT CAPTURE CHANNEL SERVICES   *
*                                    *
*************************************/

/*
 * @brief Configures FlexTimer's channel as Input Capture. After this, if FlexTimer instance
 * 		  is already running, you only have to register a callback for the channel event.
 * @param instance		FTM Instance
 * @param channel		FTM Channel
 * @param mode			FTM Input Capture Mode
 */
void FTM_CH_IC_Init(FTM_Module_t instance, FTM_Channel_t channel, FTM_IC_Mode_t mode);

/****************************************
*                                       *
*    OUTPUT COMPARE CHANNEL SERVICES    *
*                                       *
****************************************/
void CH0_Callback (uint16_t CnV);
void CH0_OutputCompare(uint16_t count, FTM_OC_Mode_t mode, bool outInit, CH_Callback_t callback);
/*
 * @brief Configures FlexTimer's channel as Output Compare. Even if the FlexTimer instance is
 *        running, the Output Compare does not start working until you call ftmOutputCompareStart()
 *        which enables the matching process between the counter registers of the instance and the channel.
 *        This allows you to control when the output compare is working.
 * @param instance		FTM Instance
 * @param channel		FTM Channel
 * @param mode			FTM Output Compare mode
 * @param outInit		FTM Output initial value when starting the operation
 */
void FTM_CH_OC_Init(FTM_Module_t module, FTM_Channel_t channel, FTM_OC_Mode_t mode, bool outInit);

/**
 * @brief Starts running the Output Compare, enables the match process.
 * @param instance		FTM Instance
 * @param channel		FTM Channel
 * @param count			FTM ticks to wait until matching process
 */
void FTM_CH_OC_Start(FTM_Module_t instance, FTM_Channel_t channel, uint16_t count);

/**
 * @brief Stops running the Output Compare mode, disables the match process.
 * @param instance		FTM Instance
 * @param channel		FTM Channel
 */
void FTM_CH_OC_Stop(FTM_Module_t instance, FTM_Channel_t channel);

/*****************************
*                            *
*    PWM CHANNEL SERVICES    *
*                            *
*****************************/

/*
 * @brief Configures FlexTimer instance and channel as PWM, starts running whenever the FlexTimer
 *        instance starts.
 * @param instance		FTM Instance
 * @param channel		FTM Channel
 * @param mode			PWM mode
 * @param alignment		PWM Alignment
 * @param duty			Duty cycle ticks count
 * @param period		Period ticks count
 */
void FTM_CH_PWM_Init(FTM_Module_t instance, FTM_Channel_t channel, FTM_PWM_Mode_t mode, FTM_PWM_Align_t alignment, uint16_t duty, uint16_t period);

/*
 * @brief Updates the current duty value of the channel configured as PWM, synchronization process is triggered
 * 		  and handled automatically.
 * @param instance		FTM Instance
 * @param channel		FTM Channel
 * @param duty			Duty cycle ticks count
 */
void FTM_PWM_SetDuty(FTM_Module_t module, FTM_Channel_t channel, uint16_t duty);

/**
 * @brief Sets the enable status of the channel output, even if the PWM is still working
 * 		  the output is not driven by its state when disabled. Can be useful for turning off
 * 		  the PWM without changing it current duty value.
 * @param instance		FTM Instance
 * @param channel		FTM Channel
 * @param running		True if enabled, false if disabled (output is masked)
 */
void FTM_PWM_ON(FTM_Module_t module, FTM_Channel_t channel, bool running);


/*******************************************************************************
 ******************************************************************************/

#endif
