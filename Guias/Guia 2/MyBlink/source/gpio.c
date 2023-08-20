/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Bruno Di Sanzo
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "gpio.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define PORT(port)							((PORT_Type *) (PORTA_BASE + 0x00001000u * (port)))

#define SIM_SCGC5_PORT_SHIFT(port)			(9U + (uint32_t)(port))
#define SIM_SCGC5_PORT_MASK(port)			((uint32_t)(1U << SIM_SCGC5_PORT_SHIFT((port))))
#define SIM_SCGC5_PORT(x, port) 			(((uint32_t)((uint32_t) (x) << SIM_SCGC5_PORT_SHIFT((port)))) & SIM_SCGC5_PORT_MASK((port)))

#define GPIO(p)								((GPIO_Type *) (GPIOA_BASE + (p) * 0x00000040u))

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
} PORTEvent_t;

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
void gpioMode (pin_t pin, uint8_t mode)
{
	uint8_t port = PIN2PORT(pin);
	uint8_t pin_number = PIN2NUM(pin);

	//1-Enable clocking for ports
	SIM->SCGC5 |= SIM_SCGC5_PORT_MASK(port);

	//2-Configure port control register bits (pin properties)

	PORT(port)->PCR[pin_number]=0x0; //Clear all bits
	PORT(port)->PCR[pin_number]|=PORT_PCR_MUX(PORT_mGPIO); 		//Set MUX to GPIO
	//PORT(port)->PCR[pin_number]|=PORT_PCR_DSE(1);          		//Drive strength enable=Slow rate
	PORT(port)->PCR[pin_number]|=PORT_PCR_IRQC(PORT_eDisabled);  //Disable interrupts

	// Configure GPIO DDR registers and PullUp/PullDown
	switch(mode)
	{
	case INPUT_PULLUP:
		GPIO(port)->PDDR &= ~(1 << pin_number);	// leave the other bits unaffected
		PORT(port)->PCR[pin_number]|=PORT_PCR_PE(1);          		//Pull UP/Down  Enable
		PORT(port)->PCR[pin_number]|=PORT_PCR_PS(1);          		//Pull UP
		break;

	case INPUT_PULLDOWN:
		GPIO(port)->PDDR &= ~(1 << pin_number);	// leave the other bits unaffected
		PORT(port)->PCR[pin_number]|=PORT_PCR_PE(1);          		//Pull UP/Down  Enable
		PORT(port)->PCR[pin_number]|=PORT_PCR_PS(0);          		//Pull Down
		break;

	case INPUT:
		GPIO(port)->PDDR &= ~(1 << pin_number);	// leave the other bits unaffected
		break;

	case OUTPUT:
		GPIO(port)->PDDR |= (1 << pin_number);		// leave the other bits unaffected
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
