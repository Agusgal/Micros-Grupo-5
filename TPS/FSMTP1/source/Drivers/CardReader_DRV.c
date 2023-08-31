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
#define	PIN_CLOCK  		2     	//PTA0
#define PORT_CLOCK		PB

#define	PIN_DATA		3		//PTA1
#define PORT_DATA		PB

#define PIN_ENABLE		10		//PTA2
#define PORT_ENABLE		PB

#define	BIT_MASK(b)		((uint8_t)(~(0b11111111<<(b))))// To be used to obtain the (b-1) least significant bits with &

#define SEQUENCE_MASK	((uint8_t)0b00011111)
#define NEW_BIT_POSITION	5

static uint8_t cardState = IDLE;
static uint8_t data [40];
static uint8_t number_of_characters = 0;
static uint8_t tempData = 0;
static uint8_t new_bit_position = 0;

// For timer
#define TIME_CONSTANT		1000000	// 0.1 segs
#define	TIMER_RESET_VALUE	10 // 1 segs

static uint32_t counter = 0;

static bool	data_ready = false;

// For debugging
//#define	DEBUG_CARD
#ifdef	DEBUG_CARD
static uint8_t aux [32];
static uint8_t aux_counter = 0;
#endif

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
	gpioMode(PORTNUM2PIN(PORT_DATA, PIN_DATA), INPUT);

	// Enable pin
	gpioMode(PORTNUM2PIN(PORT_ENABLE, PIN_ENABLE), INPUT);

	SysTick_Reg_Callback(cardReader_PISR, TIME_CONSTANT);

	return true;

}

/**
 * @brief Initialize CardReader driver
 * @return 2 if the data is ready, 1 if the data is being read, 0 if no data was obtained.
 */
bool getCardReader_Status(void)
{
	return data_ready;
}

/**
 * @brief Get the card data
 * @param data Pointer to an array that will store the data from the card (at least 40 bytes needed)
 * @return True if data could be successfully read, False otherwise
 */
void getCardReader_Data(uint8_t *data_buffer)
{
	uint8_t i;
	for(i = 0; i < NUMBER_OF_CHARACTERS; i++)
	{
		data_buffer[i] = data[i];
	}
}

/**
 * @brief Check if data is valid with the LRC
 * @param data Pointer to an array that stores the data from the card (at least 40 bytes)
 * @return True if data could be successfully read, False otherwise
 */
bool checkLRC_CardReader_Data(uint8_t *data_buffer, uint8_t number_of_characters)
{
	uint8_t i;
	uint8_t aux = data_buffer[0];
	uint8_t max_number_of_characters = (number_of_characters < NUMBER_OF_CHARACTERS) ?  number_of_characters : NUMBER_OF_CHARACTERS;
	bool ESFound = false;

	// Read
	for(i = 1; i < max_number_of_characters && !ESFound; i++)
	{
		aux ^= data_buffer[i];
		if(data_buffer[i] == ES)
		{
			ESFound = true;
		}
	}

	//If no End Sentinel Found or there is no space for LRC, return false
	if(!ESFound || i > max_number_of_characters - 1)
	{
		return false;
	}

	// Check the parity of the LRC (if incorrect, return false)
	uint8_t j;
	uint8_t lrc_displaced = data_buffer[i];
	uint8_t check_lrc = lrc_displaced & 0b00000001;	// get last bit
	for (j = 0; j < 3; j++)
	{
		check_lrc ^= ((lrc_displaced >>= 1) & 0b00000001);
	}

	// The xor of the last 4 bits should NOT be equal to the fifth bit (example 1 1001)
	if(check_lrc == (lrc_displaced >>= 1))
	{
		return false;
	}
	// Compare LRC with aux (except parity check bit)
	return (data_buffer[i] & 0b00001111) == (aux & 0b00001111);

}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
__ISR__ PORTB_IRQHandler(void)
{
	// Clear interrupt flag
	gpio_clear_interrupt_flag(PORTNUM2PIN(PORT_CLOCK, PIN_CLOCK));

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
			tempData >>= 1;

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
			tempData = (tempData & BIT_MASK(new_bit_position)) | (!gpioRead(PORTNUM2PIN(PORT_DATA, PIN_DATA)) << new_bit_position);

			new_bit_position++;
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
			tempData = (tempData & BIT_MASK(new_bit_position)) | (!gpioRead(PORTNUM2PIN(PORT_DATA, PIN_DATA)) << new_bit_position);

			new_bit_position++;
			//Store character if 5 digits were detected (should be LRC)
			if(new_bit_position == 5 && number_of_characters < NUMBER_OF_CHARACTERS)
			{
				data[number_of_characters] = tempData;
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
