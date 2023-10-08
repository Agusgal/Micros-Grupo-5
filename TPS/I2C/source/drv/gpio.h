/***************************************************************************//**
  @file     gpio.h
  @brief    Simple GPIO Pin services, similar to Arduino
  @author   Grupo 5
 ******************************************************************************/

#ifndef _GPIO_H_
#define _GPIO_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "MK64F12.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// Ports
enum { PA, PB, PC, PD, PE };

// Convert port and number into pin ID
// Ex: PTB5  -> PORTNUM2PIN(PB,5)  -> 0x25
//     PTC22 -> PORTNUM2PIN(PC,22) -> 0x56
#define PORTNUM2PIN(p,n)    (((p)<<5) + (n))
#define PIN2PORT(p)         (((p)>>5) & 0x07)
#define PIN2NUM(p)          ((p) & 0x1F)

#define LED_GREEN_PIN	PORTNUM2PIN(PE,26)
#define LED_RED_PIN		PORTNUM2PIN(PB,22)
#define LED_BLUE_PIN	PORTNUM2PIN(PB,21)
#define I2C_TP_PIN		PORTNUM2PIN(PC,10)

// Modes
#ifndef INPUT
#define INPUT               0
#define OUTPUT              1
#define INPUT_PULLUP        2
#define INPUT_PULLDOWN      3
#endif // INPUT


// Digital values
#ifndef LOW
#define LOW     0
#define HIGH    1
#endif // LOW


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef uint8_t pin_t;

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

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Configures the specified pin to behave either as an input or an output
 * @param pin the pin whose mode you wish to set (according PORTNUM2PIN)
 * @param mode INPUT, OUTPUT, INPUT_PULLUP or INPUT_PULLDOWN.
 */
void gpioMode (pin_t pin, uint8_t mode);

/**
 * @brief Write a HIGH or a LOW value to a digital pin
 * @param pin the pin to write (according PORTNUM2PIN)
 * @param val Desired value (HIGH or LOW)
 */
void gpioWrite (pin_t pin, bool value);

/**
 * @brief Toggle the value of a digital pin (HIGH<->LOW)
 * @param pin the pin to toggle (according PORTNUM2PIN)
 */
void gpioToggle (pin_t pin);

/**
 * @brief Reads the value from a specified digital pin, either HIGH or LOW.
 * @param pin the pin to read (according PORTNUM2PIN)
 * @return HIGH or LOW
 */
bool gpioRead (pin_t pin);

/**
 * @brief				Set IRQ settings for the specified pin
 * @param pin			The pin to configure
 * @param irq_config	The IRQ configuration
 */
void gpioIRQ_Config (pin_t pin, PORTEvent_t irq_config);

/**
 * @brief				Clear interrupt flag
 * @param pin			The pin to configure
 */
void gpio_clear_interrupt_flag(pin_t pin);

/**
 * @brief
 */
//bool gpioFlank(pin_t pin, bool active);


/*******************************************************************************
 ******************************************************************************/

#endif // _GPIO_H_
