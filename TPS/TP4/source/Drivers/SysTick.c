/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Bruno Di Sanzo
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "SysTick.h"
#include "hardware.h"
#include "Drivers/Display.h"
#include "gpio.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define TEST_PIN_2      10
#define TEST_PORT_2     PC

typedef enum
{
	PORT_mAnalog,
	PORT_mGPIO,
	PORT_mAlt2,
	PORT_mAlt3,
	PORT_mAlt4,
	PORT_mAlt5,
	PORT_mAlt6,
	PORT_mAlt7,

} PORTMux_t;

/*
typedef enum
{
	PORT_eDisabled				= 0x00,
	PORT_eDMARising				= 0x01,
	PORT_eDMAFalling			= 0x02,
	PORT_eDMAEither				= 0x03,
	PORT_eInterruptDisasserted	= 0x08,
	PORT_eInterruptRising		= 0x09,
	PORT_eInterruptFalling		= 0x0A,
	PORT_eInterruptEither		= 0x0B,
	PORT_eInterruptAsserted		= 0x0C,
} PORTEvent_t;*/


typedef struct SysTick_Callback_Element
{
	void (*fun_Callback)(void);
	uint32_t num_Cycles;
	uint32_t counter;
}SysTick_Callback_Element_Type;


// Array of stored callback functions
static SysTick_Callback_Element_Type systick_Callback_Array [TOTAL_NUM_CALLBACK_FUNCTIONS];
static uint32_t num_Callbacks = 0;


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
__ISR__ SysTick_Handler(void);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/**
 * @brief Initialize SysTic driver
 * @return Initialization and registration succeed
 */
void SysTick_Init ()
{
	SysTick->CTRL = 0x00;
	SysTick->LOAD = (__CORE_CLOCK__/ S_TO_US) * SYSTICK_ISR_PERIOD_US - 1; //12499999L; // <= 125 ms @ 100Mhz
	SysTick->VAL  = 0x00;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

	gpioMode(PORTNUM2PIN(TEST_PORT_2, TEST_PIN_2), OUTPUT);
}

/**
 * @brief Initialize SysTic driver
 * @param funcallback Function to be call every SysTick
 * @param period Period in which the function will be called in microseconds (us)
 * @return Initialization and registration succeed
 */
bool SysTick_Reg_Callback (void (*funCallback)(void), uint32_t period)
{
	uint32_t num_Cycles = period / SYSTICK_ISR_PERIOD_US;

	if(num_Callbacks > TOTAL_NUM_CALLBACK_FUNCTIONS - 1 || num_Cycles == 0)
	{
		return false;
	}

	SysTick_Callback_Element_Type element = {funCallback, num_Cycles, num_Cycles};
	systick_Callback_Array[num_Callbacks++] = element;
	return true;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/*
__ISR__ SysTick_Handler(void)
{

	//test pin high
	gpioWrite(PORTNUM2PIN(TEST_PORT_2, TEST_PIN_2), HIGH);

	// Iterate through all the callbacks
	for (uint32_t i = 0; i < num_Callbacks; i++)
	{
		systick_Callback_Array[i].counter--;
		if (!systick_Callback_Array[i].counter) //If the counter reaches 0
		{
			(*systick_Callback_Array[i].fun_Callback)(); // Callback's calling.
			systick_Callback_Array[i].counter = systick_Callback_Array[i].num_Cycles;	  //Counter re-establishment.
		}
	}

	//Test pin low
	gpioWrite(PORTNUM2PIN(TEST_PORT_2, TEST_PIN_2), LOW);
}

*/

/*******************************************************************************
 ******************************************************************************/
