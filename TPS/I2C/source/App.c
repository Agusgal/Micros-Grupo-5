/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>
#include "gpio.h"
#include <i2c.h>
#include <accel.h>
#include <FXOS8700CQ.h>
#include <math.h>
#include "SysTick.h"
#include "comunicationHandler.h"
#include "drv/Uart.h"
#include "drv/CAN_SPI.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

enum{ROLL_REFRESH, PITCH_REFRESH, ORIENT_REFRESH};
uint8_t fsm = ROLL_REFRESH;

Orient_t AppBoard;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void periodicRefresh(void);
bool getCAN_Event(void);
void enableSend();



/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void periodicRefresh(void)
{
	Orient_t accel_data = getAccelData();
	if (fabs(accel_data.rolido-AppBoard.rolido)>=5)
		setWriteAvailable(ROLL_REFRESH,true);
	if (fabs(accel_data.cabeceo-AppBoard.cabeceo)>=5)
		setWriteAvailable(PITCH_REFRESH,true);
	if (fabs(accel_data.norte-AppBoard.norte)>=5)
		setWriteAvailable(ORIENT_REFRESH,true);
	// Cada 1 segundo refresca uno de los parámetros
	switch (fsm)
	{
		case ROLL_REFRESH:
			AppBoard.rolido = accel_data.rolido;
			comunicationHandler_send2Ext(AppBoard, ROLL_UPD);
			break;
		case PITCH_REFRESH:
			AppBoard.cabeceo = accel_data.cabeceo;
			comunicationHandler_send2Ext(AppBoard, PITCHING_UPD);
			break;
		case ORIENT_REFRESH:
			AppBoard.norte = accel_data.norte;
			comunicationHandler_send2Ext(AppBoard, ORIENTATION_UPD);
			break;
	}


	if(fsm == ORIENT_REFRESH)
	{
		fsm = ROLL_REFRESH;
	}
	else
	{
		fsm++;
	}
}


/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
    SysTick_Init();

	FX_I2C_Init();

	comunicationHandler_init(5);

    SysTick_Reg_Callback(periodicRefresh, 50 * MS_TO_US);
    SysTick_Reg_Callback(enableSend, 1000 * MS_TO_US);


}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	if(getCAN_Event())
	{
		Com_EventHandler();
	}
}


bool getCAN_Event(void)
{

	//CAN_Com_Happened es un servicio de can que indica si hubo evento de recebcion o envio de datos de CAN.
	bool a = CAN_SPI_Is_Read_Ready();
	return CAN_SPI_Is_Read_Ready();
}

void enableSend()
{
	setWriteAvailable(1,true);
	setWriteAvailable(2,true);
	setWriteAvailable(3,true);
}
/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/*******************************************************************************
 ******************************************************************************/
