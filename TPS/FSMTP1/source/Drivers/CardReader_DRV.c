/***************************************************************************//**
  @file     CardReader_DRV.c

  @brief    Card Reader driver
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "SysTick.h"
#include "gpio.h"
#include "CardReader_DRV.h"
#include "hardware.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define	PIN_CLOCK  		0     	//PTA0
#define PORT_CLOCK		PA

#define	PIN_DATA		1		//PTA1
#define PORT_DATA		PA

#define PIN_ENABLE		2		//PTA2
#define PORT_ENABLE		PA

#define	BIT_MASK(b)		(~(0b11111111<<(b)))// To be used to obtain the (b-1) least significant bits with &

#define SEQUENCE_MASK	0b00011111
#define NEW_BIT_POSITION	5

static uint8_t cardState = IDLE;
static uint8_t data [40];
static uint8_t number_of_characters = 0;
static uint8_t tempData = 0;
static uint8_t new_bit_position;

// For timer
#define TIME_CONSTANT		100000	// 0.1 segs
#define	TIMER_RESET_VALUE	1 // 1 segs

static uint32_t counter = 0;
static uint32_t start_timer = false;

static bool	data_ready = false;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
*******************************************************************************/
static void cardReader_PISR(void);


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
/**
 * @brief Initialize CardReader driver
 * @return Initialization succeed
 */
bool cardReader_Init(void)
{
	// Clock pin
	gpioMode(PORTNUM2PIN(PORT_CLOCK, PIN_CLOCK), INPUT);
	gpioIRQ_Config(PORTNUM2PIN(PORT_CLOCK, PIN_CLOCK), PORT_eInterruptFalling);

	// Data pin
	gpioMode(PORTNUM2PIN(PORT_CLOCK, PIN_CLOCK), INPUT);

	// Enable pin
	gpioMode(PORTNUM2PIN(PORT_CLOCK, PIN_CLOCK), INPUT);

	SysTick_Reg_Callback(cardReader_PISR, TIME_CONSTANT);

	return true;

}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
__ISR__ PORTA_IRQHandler(void)
{
	// If there is a clock event, reset the timer that erases everything every 1sec
	counter = TIMER_RESET_VALUE;

	//Check for enable (ACTIVE LOW)
	if(gpioRead(PORTNUM2PIN(PORT_ENABLE, PIN_ENABLE)))
	{
		tempData = 0;
		cardState = IDLE;
		number_of_characters = 0;
		new_bit_position = 0;
		data_ready = false;
	}
	else
	{
		switch(cardState)
		{

		case IDLE:
			// Store the new bit (ACTIVE LOW) in the fifth position and shift the byte
			tempData = (tempData & SEQUENCE_MASK) | (!gpioRead(PORTNUM2PIN(PORT_DATA, PIN_DATA)) << NEW_BIT_POSITION);
			tempData <<= 1;

			// If Start Sentinel is found, store it and change state
			if(tempData == SS)
			{
				data[0] = tempData;
				number_of_characters++;
				cardState = READING;
			}

			break;

		case READING:
			// Store the new bit (ACTIVE LOW) in the free position
			tempData = (tempData & BIT_MASK(new_bit_position)) | (!gpioRead(PORTNUM2PIN(PORT_DATA, PIN_DATA)) << new_bit_position++);

			//Store character if 5 digits were detected
			if(new_bit_position == 5 && number_of_characters < NUMBER_OF_CHARACTERS)
			{
				data[number_of_characters++] = tempData;
				new_bit_position = 0;

				// Check for End Sentinel
				if(tempData == ES)
				{
					cardState = FINISH;
				}
			}
			break;

		case FINISH:
			tempData = (tempData & BIT_MASK(new_bit_position)) | (!gpioRead(PORTNUM2PIN(PORT_DATA, PIN_DATA)) << new_bit_position++);

			//Store character if 5 digits were detected (should be LRC)
			if(new_bit_position == 5 && number_of_characters < NUMBER_OF_CHARACTERS)
			{
				data[number_of_characters] = tempData;
				number_of_characters = 0;
				new_bit_position = 0;
				data_ready = true;
				// Wait for timer to restart the state machine
			}
			break;

		default:
			break;
		}
	}

}

/**
 * @brief	Timer
 */
static void cardReader_PISR(void)
{
	if(counter)
	{
		counter--;
		// If counter reaches 0, restart everything
		if(!counter)
		{
			tempData = 0;
			cardState = IDLE;
			number_of_characters = 0;
			new_bit_position = 0;
			data_ready = false;
		}
	}

}


/*******************************************************************************
 ******************************************************************************/
