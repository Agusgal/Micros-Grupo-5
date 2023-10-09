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

#include "SysTick.h"
#include "comunicationHandler.h"
#include "drv/Uart.h"

//#include "drv/CAN.h"

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
void testRolido(void);
void testCabeceo(void);
void testNorte(void);
bool getCAN_Event(void);



bool testVar = false;
void testFunc(void);
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void periodicRefresh(void)
{
	int a = 1;
	// Cada 1 segundo refresca uno de los parámetros
	switch (fsm)
	{
		case ROLL_REFRESH:
			AppBoard.rolido = getAccelData().rolido;
			comunicationHandler_send2Ext(AppBoard, ROLL_UPD);
			break;
		case PITCH_REFRESH:
			AppBoard.cabeceo = getAccelData().cabeceo;
			comunicationHandler_send2Ext(AppBoard, PITCHING_UPD);
			break;
		case ORIENT_REFRESH:
			AppBoard.norte = getAccelData().norte;
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

void testRolido(void)
{
	AppBoard = getAccelData();
	int16_t boardDATA;

	char buffer[7];
	testParser(buffer, &boardDATA, 1, AppBoard);
	UART_SendMsg(buffer, 0);
}

void testCabeceo(void)
{
	AppBoard = getAccelData();
	int16_t boardDATA;

	char buffer1[7];
	testParser(buffer1, &boardDATA, 2, AppBoard);
	UART_SendMsg(buffer1, 0);
}


void testNorte(void)

{
	AppBoard = getAccelData();
	int16_t boardDATA;

	char buffer2[7];
	testParser(buffer2, &boardDATA, 3, AppBoard);
	UART_SendMsg(buffer2, 0);
}



/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
    SysTick_Init();

	FX_I2C_Init();

	comunicationHandler_init(5);

    SysTick_Reg_Callback(periodicRefresh, 1000 * MS_TO_US);

    //solo prueba
    //SysTick_Reg_Callback(testRolido, 500 * MS_TO_US);
    //SysTick_Reg_Callback(testCabeceo, 600 * MS_TO_US);
    //SysTick_Reg_Callback(testNorte, 700 * MS_TO_US);
    //SysTick_Reg_Callback(testFunc, 1000 * MS_TO_US);


}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	if(!testVar)
	{
		Com_EventHandler();
		testVar = !testVar;
	}
}


bool getCAN_Event(void)
{
	//CAN_Com_Happened es un servicio de can que indica si hubo evento de recebcion o envio de can
	//return CAN_Com_Happened();
}

void testFunc(void)
{
	testVar = !testVar;
}
/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/*******************************************************************************
 ******************************************************************************/
