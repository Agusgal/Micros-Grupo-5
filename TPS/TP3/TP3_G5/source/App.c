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
#define NULL 0
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
void changeDuty (void);
void IC_Fun (uint16_t CnV);

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
	FTM0_Init(FTM_PSC_x32,&changeDuty);
	FTM_Init(FTM_3,FTM_PSC_x32,0xFFFF,NULL);
    FTM_CH_PWM_Init(FTM_0, FTM_CH_0, FTM_PWM_HIGH_PULSES, FTM_PWM_EDGE_ALIGNED, duty, period,NULL);		//90% duty cycle (en hexa)
    FTM_CH_IC_Init(FTM_3, FTM_CH_5,FTM_IC_BOTH_EDGES,&IC_Fun);
    FTM_Restart(FTM_0);
    FTM_Restart(FTM_3);
    FTM_PWM_ON(FTM_0,FTM_CH_0);

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

void IC_Fun (uint16_t CnV)
{
	static uint16_t med1,med2,med;
	static uint8_t  state=0;

	if(state==0)
	{
		med1=CnV;
		state=1;
	}
	else if(state==1)
	{
		med2=CnV;
		med=med2-med1;
		state=0;					// Set break point here and watch "med" value
	}
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/*******************************************************************************
 ******************************************************************************/
