/*
 * comunicationHandler.c
 *
 *  Created on: 8 oct. 2023
 *      Author: Grupo 5
 */




#include "comunicationHandler.h"
//#include "drv/SPI.h"
//#include "drv/CAN.h"
#include "drv/UART.h"

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
#define R_CHAR		0x82
#define C_CHAR		0x67
#define O_CHAR		0x79
#define PLUS_CHAR	0x43
#define MINUS_CHAR	0x45


/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/
static Orient_t Boards[GROUPS];
static uint8_t myGroup;
static char bufferPC[7] = {'5', 'R', '+', '1', '2', '3', '\0'}; //TOdo: ???????

typedef struct
{
	uint16_t SID;
	uint8_t DLC;
	uint8_t Dn[8];
}CAN_RAWDATA_t;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


//int16_t ComHandler_parseCANmsj(char* buf, int16_t* boardDATA, uint8_t typeUPD, Orient_t myBoard);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void comunicationHandler_init(uint8_t group_num)
{
	myGroup = group_num;

	//Todo: reemplazar con los posta
	//SPI_Init();
	//init_CAN(myGroup, ExternManager_EventHandler);

	UART_Init();

	// Borrar lo de abajo es prueba
    //char buf[4] = {'O', '-', 4, 7};
    //send_CAN(0x105, buf, 4);
}

void Com_EventHandler(void)
{

	//getter de la informacion se CAN que llego, llamo un servicio del driver de can
	//CAN_RAWDATA_t bufferRXB = CAN_getRawData();

	//TEST
	uint16_t sid = 0x105;
	uint8_t dlc = 0x4;

	CAN_RAWDATA_t bufferRXB = {.SID=sid, .DLC=dlc, {'R', '+', '2', '5'}};



	uint8_t group_index = ((bufferRXB.SID) & 0x00F) - 1;
	uint8_t digitos = bufferRXB.DLC - 2;
	uint8_t angle;

	if(digitos == 1)
	{
		angle = bufferRXB.Dn[2] - '0';
	}
	else if(digitos == 2)
	{
		angle = (bufferRXB.Dn[2] - '0')*10 + (bufferRXB.Dn[3] - '0');
	}
	else if(digitos == 3)
	{
		angle = (bufferRXB.Dn[2] - '0')*100 + (bufferRXB.Dn[3] - '0')*10 + (bufferRXB.Dn[4] - '0');
	}

	switch (bufferRXB.Dn[1])
	{
		case '+':
			break;
		case '-':
			angle = -angle;
			break;
		default:
			// Si no es + ni - seguro es un 0
			angle = 0;
			break;
	}

	switch (bufferRXB.Dn[0])
	{
		case 'R':
			Boards[group_index].rolido = angle;
			break;
		case 'C':
			Boards[group_index].cabeceo = angle;
			break;
		case 'O':
			Boards[group_index].norte = angle;
			break;
	}


	// Refresco de PC y formateo para enviar a python
	bufferPC[0] = group_index;
	bufferPC[1] = bufferRXB.Dn[0];
	if(bufferRXB.Dn[1] == 0)
	{
		bufferPC[2] = PLUS_CHAR;
		bufferPC[3] = 0;
		bufferPC[4] = 0;
		bufferPC[5] = 0;
	}
	else
	{
		if(bufferRXB.Dn[1] == '+')
		{
			bufferPC[2] = '+';
		}
		else if(bufferRXB.Dn[1] == '-')
		{
			bufferPC[2] = '-';
		}


		bufferPC[3] = ((angle / 100) % 10) + '0';
		bufferPC[4] = ((angle / 10) % 10) + '0';
		bufferPC[5] = (angle % 10) + '0';

	}


	//Todo: Si algo falla probablemente este aca
	UART_SendMsg(bufferPC, 0);

}

int16_t ComHandler_parseCANmsj(char* buf, int16_t* boardDATA, uint8_t typeUPD, Orient_t myBoard)
{
	int16_t nBytes;

	switch (typeUPD) {
		case ROLL_UPD:
			*boardDATA = myBoard.rolido;
			buf[0] = 'R';
			break;
		case PITCHING_UPD:
			*boardDATA = myBoard.cabeceo;
			buf[0] = 'C';
			break;
		case ORIENTATION_UPD:
			*boardDATA = myBoard.norte;
			buf[0] = 'O';
			break;
	}

	if(boardDATA == 0)
	{
		buf[1] = 0;
		nBytes = 2;
	}
	else
	{
		// Defino signo
		if(*boardDATA > 0)
		{
			buf[1] = '+';
		}
		else
		{
			buf[1] = '-';
			*boardDATA = -(*boardDATA);
		}


		// Defino digitos
		uint8_t c, d, u;
		c = ((*boardDATA / 100) % 10) + '0';
		d = ((*boardDATA / 10) % 10) + '0';


		u = (*boardDATA % 10) + '0';

		if(c != '0')
		{
			buf[2] = c;
			buf[3] = d;
			buf[4] = u;
			nBytes = 5;
		}
		else
		{
			if(d != '0')
			{
				buf[2] = d;
				buf[3] = u;
				nBytes = 4;
			}
			else
			{
				buf[2] = u;
				nBytes = 3;

			}
		}
	}

	return nBytes;
}

void comunicationHandler_send2Ext(Orient_t myBoard, uint8_t typeUPD)
{
	int16_t boardDATA, nBytes;
	char buffer[5]; // Considero peor caso, pero cuando mando indico cuantos bytes son

	nBytes = ComHandler_parseCANmsj(buffer, &boardDATA, typeUPD, myBoard);

	//todo: cambiar por el posta
	//send_CAN(0x105, buffer, nBytes);

	Boards[myGroup].rolido = myBoard.rolido;
	Boards[myGroup].cabeceo = myBoard.cabeceo;
	Boards[myGroup].norte = myBoard.norte;
}


int16_t testParser(char* buf, int16_t* boardDATA, uint8_t typeUPD, Orient_t myBoard)
{
	int16_t nBytes;
	buf[0] = '5';
	int angle;
	switch (typeUPD) {
		case ROLL_UPD:
			angle = myBoard.rolido;
			buf[1] = 'R';
			break;
		case PITCHING_UPD:
			angle = myBoard.cabeceo;
			buf[1] = 'C';
			break;
		case ORIENTATION_UPD:
			angle = myBoard.norte;
			buf[1] = 'O';
			break;
	}

	if(boardDATA == 0)
	{
		buf[1] = 0;
		nBytes = 2;
	}
	else
	{
		// Defino signo
		if(angle > 0)
		{
			buf[2] = '+';
		}
		else
		{
			buf[2] = '-';
			angle = -(angle);
		}



	// Refresco de PC y formateo para enviar a python

	buf[5] = (angle % 10) + '0';
	buf[4] = ((angle / 10) % 10) + '0';
	buf[3] = ((angle / 100) % 10) + '0';
	buf[6] = '\0';
	}


	return nBytes;
}

/*******************************************************************************
 ******************************************************************************/
