/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "MK64F12.h"
#include "hardware.h"

#include "Drivers/board.h"
#include "Drivers/gpio.h"
#include "Drivers/dma_drv.h"
#include "Drivers/FTM.h"
#include "Drivers/port.h"


#include "EventQueue/queue.h"
#include "FSM/FSM.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
void changeDuty (void);

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
int duty = 0xE66;
int period = 0x1000;

uint16_t sourceBuffer[] = {1000 ,2000,3000,4000,5000,6000,7000,8000};

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{

	PORT_Init();
	FTM0_Init(2, 0);
    FTM_CH_PWM_Init(FTM_0, FTM_CH_0, FTM_PWM_HIGH_PULSES, FTM_PWM_EDGE_ALIGNED, duty, period);		//90% duty cycle (en hexa)
    FTM_Restart(FTM_0);
    FTM_PWM_ON(FTM_0,FTM_CH_0, true);

    uint16_t * CnV_pointer = FTM_CH_GetCnVPointer(FTM_0, FTM_CH_0);
    dma0_init(FTM0CH0, 0, sourceBuffer, CnV_pointer, 2, 0, 2, 2, 8, sizeof(sourceBuffer), 0);

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
