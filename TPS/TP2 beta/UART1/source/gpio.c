/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Grupo 5
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

//enum { IDLE=0, PRESSED, RELEASED};

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

/**
 * @brief				Set IRQ settings for the specified pin
 * @param pin			The pin to configure
 * @param irq_config	The IRQ configuration
 */
void gpioIRQ_Config (pin_t pin, PORTEvent_t irqMode)
{
	NVIC_EnableIRQ(PORTA_IRQn + PIN2PORT(pin));
	PORT(PIN2PORT(pin))->PCR[PIN2NUM(pin)] = (PORT(PIN2PORT(pin))->PCR[PIN2NUM(pin)] & ~PORT_PCR_IRQC_MASK) | PORT_PCR_IRQC(irqMode);
}

/**
 * @brief				Clear interrupt flag
 * @param pin			The pin to configure
 */
void gpio_clear_interrupt_flag(pin_t pin)
{
	PORT(PIN2PORT(pin))->PCR[PIN2NUM(pin)] |= PORT_PCR_ISF_MASK;
}

/**
 * @brief
 */
/*
bool gpioFlank(pin_t pin, bool active)		// Detector de Flancos en pines de input
{
	static bool state = IDLE;
	bool change=LOW;
	bool pinRead;

	pinRead = gpioRead(pin);

	switch (state)
	{
		case IDLE:
			if (pinRead==active){
				state=PRESSED;
				change=HIGH;
				}
				break;
		case PRESSED:
			if (pinRead==!active)
				state=IDLE;
			break;
	}
	return change;
}*/

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



/*******************************************************************************
 ******************************************************************************/
