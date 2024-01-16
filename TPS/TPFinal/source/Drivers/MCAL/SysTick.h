/***************************************************************************//**
  @file     SysTick.h
  @brief    SysTick driver
  @author   Nicol�s Magliola
 ******************************************************************************/

#ifndef _SYSTICK_H_
#define _SYSTICK_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>
#include <stdint.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define TOTAL_NUM_CALLBACK_FUNCTIONS	15	// Total number of callback functions
#define SYSTICK_ISR_PERIOD_US 1000U		// Period of SysTick in microseconds
#define SYSTICK_ISR_FREQUENCY_HZ 1000U
#define S_TO_US		1000000
#define MS_TO_US	1000
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
 * @brief Initialize SysTic driver
 * @return Initialization and registration succeed
 */
void SysTick_Init ();

/**
 * @brief Initialize SysTic driver
 * @param funcallback Function to be call every SysTick
 * @param period Period in which the function will be called in microseconds (us)
 * @return Initialization and registration succeed
 */
bool SysTick_Reg_Callback (void (*funCallback)(void), uint32_t period);


/*******************************************************************************
 ******************************************************************************/

#endif // _SYSTICK_H_
