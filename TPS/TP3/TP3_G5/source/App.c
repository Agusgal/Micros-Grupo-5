/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "gpio.h"
#include "port.h"
#include "FTM.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
void changeDuty (void);

int duty = 0xE66;
int period = 0x1000;
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{

	GPIO_Init();
	PORT_Init();
	FTM0_Init(2,&changeDuty);
    FTM_CH_PWM_Init(FTM_0, FTM_CH_0, FTM_PWM_HIGH_PULSES, FTM_PWM_EDGE_ALIGNED, duty, period);		//90% duty cycle (en hexa)
    FTM_Restart(FTM_0);
    FTM_PWM_ON(FTM_0,FTM_CH_0, true);

}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{

}

// esta funcion se ejecuta cuando termina un período de FTM, va rotando de duty (solo pa testear)
void changeDuty (void)
{
	FTM_PWM_SetDuty(FTM_0,FTM_CH_0,(FTM_CH_GetCount(FTM_0,FTM_CH_0)+1)%(period));
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/*******************************************************************************
 ******************************************************************************/
