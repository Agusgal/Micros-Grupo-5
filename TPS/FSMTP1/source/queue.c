/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Bruno Di Sanzo
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "queue.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/**
 * @brief Configures the specified pin to behave either as an input or an output
 * @param pin the pin whose mode you wish to set (according PORTNUM2PIN)
 * @param mode INPUT, OUTPUT, INPUT_PULLUP or INPUT_PULLDOWN.
 */
void fsm (bool timer_sec, bool timer_min)
{
	switch(states)
	{
	case ID_ENTRY:

		if(encoder) //aca habria que llamar a alguna funcion que devuelva que se esta ingresando la contra
			states = ENCODER_ENTRY;
		else if (card)
			states = CARD_ENTRY;

		break;

	case ENCODER_ENTRY:
			if(id_encoder == ENCODER)//hay que ver como se hace esto, y como es la base de datos
				states = PIN_ENTRY;
			else
				states = RED_LED_ON;
		break;

	case CARD_ENTRY:
			if(id_card == CARD)//hay que ver como se hace esto, y como es la base de datos
				states = PIN_ENTRY;

			else
				states = RED_LED_ON;
		break;

	case PIN_ENTRY:
			if()
		break;

	case RED_LED_ON:

			break;

	case GREEN_LED_ON:

			break;

	case BRIGHTNESS:

			break;

	case WRONG_ID:

			break;

	default:
		break;
	}

}

/**
 * @brief Write a HIGH or a LOW value to a digital pin
 * @param pin the pin to write (according PORTNUM2PIN)
 * @param val Desired value (HIGH or LOW)
 */
void gpioWrite (pin_t pin, bool value)
{
	if (value)
	{
		GPIO(PIN2PORT(pin))->PSOR = (1 << PIN2NUM(pin));
	}
	else
	{
		GPIO(PIN2PORT(pin))->PCOR = (1 << PIN2NUM(pin));
	}

}

/**
 * @brief Toggle the value of a digital pin (HIGH<->LOW)
 * @param pin the pin to toggle (according PORTNUM2PIN)
 */
void gpioToggle (pin_t pin)
{
	GPIO(PIN2PORT(pin))->PTOR = (1 << PIN2NUM(pin));
}

/**
 * @brief Reads the value from a specified digital pin, either HIGH or LOW.
 * @param pin the pin to read (according PORTNUM2PIN)
 * @return HIGH or LOW
 */
bool gpioRead (pin_t pin)
{
	return (GPIO(PIN2PORT(pin))->PDIR >> PIN2NUM(pin)) & 0x00000001U;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/




/*******************************************************************************
 ******************************************************************************/
