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

#define SEQUENCE_MASK	0b00011111
#define NEW_BIT_POSITION	5

#define	SS	0b0001001
#define	FS	0b0001101
#define	ES	0b0011111

static uint8_t cardState = IDLE;
static uint8_t data [40];
static uint8_t number_Of_Characters = 0;
static uint8_t tempData = 0;
static uint8_t

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
*******************************************************************************/

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


	return true;

}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
__ISR__ PORTA_IRQHandler(void)
{
	//Check for enable (ACTIVE LOW)
	if(gpioRead(PORTNUM2PIN(PORT_ENABLE, PIN_ENABLE)))
	{
		tempData = 0;
		cardState = IDLE;
		number_Of_Characters = 0;
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
				number_Of_Characters++;
				cardState = READING;
			}

			break;

		case READING:
			tempData = (tempData & SEQUENCE_MASK) | (!gpioRead(PORTNUM2PIN(PORT_DATA, PIN_DATA)) << NEW_BIT_POSITION);
						tempData <<= 1;

			break;

		case FINISH:

			break;

		default:

			break;
		}
	}

}


/*******************************************************************************
 ******************************************************************************/
