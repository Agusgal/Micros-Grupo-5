/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>
#include "gpio.h"
#include <i2c.h>
#include <accel.h>
#include <FXOS8700CQ.h>
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

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	 gpioMode(LED_GREEN_PIN, OUTPUT);
	 gpioMode(LED_RED_PIN, OUTPUT);
	 gpioMode(LED_BLUE_PIN, OUTPUT);
	 gpioWrite(LED_GREEN_PIN,HIGH);
	 gpioWrite(LED_RED_PIN,HIGH);
	 gpioWrite(LED_BLUE_PIN,HIGH);
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{


	FX_I2C_Init();
	static Orient_t accel_data;
	accel_data=getAccelData();
	if ((accel_data.rolido<=90) & (accel_data.rolido>=(-90)))
		gpioWrite(LED_GREEN_PIN,LOW);
	else
		gpioWrite(LED_GREEN_PIN,HIGH);
	if ((accel_data.norte<=7) & (accel_data.norte>=(-7)))
		gpioWrite(LED_BLUE_PIN,LOW);
	else
		gpioWrite(LED_BLUE_PIN,HIGH);
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/*******************************************************************************
 ******************************************************************************/
