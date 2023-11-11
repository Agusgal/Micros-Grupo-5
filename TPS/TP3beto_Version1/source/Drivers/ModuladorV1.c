/***************************************************************************//**
  @file     ModulatorV1.c
  @brief    modulator functions
  @author   Grupo 5
 ******************************************************************************/

#include "ModuladorV1.h"
#include "DAC.h"
#include "PIT.h"
#include "math.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

//Cantidad de puntos en la señal senoidal digital.
#define SIN_VALUES	35
#define NEXT_SYM	2
#define START		1
#define STOP		0

#define SIN0		0
#define SIN1		1

#define ONE_FREC	1200
#define ZERO_FREC	2200

#define TOBITMASK 		10000000

typedef enum { IDLE, MSG} status_t;

//Función senoidal digital de salida
static uint16_t sinValues[SIN_VALUES];

static uint16_t cont = 0;
static uint16_t cycle = 0;
static uint8_t sinIndex = 0;

static bool bitStream[STAND_LEN];
static uint8_t msg_ptr = 0;
static status_t status = IDLE;
static float aux = 0;

const float ratio = 1.83;//(ZERO_FREC/ONE_FREC);


//Funcion de callback que se ejecuta una vez se termino de enviar la señal modulada
void (*MsgSentClb)(void);


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
void sendBit(void);

void sendSin0(void);

void sendSin1(void);

void procesBitStream(void); // El comando puede ser START o NEXT_SYM

bool parity (char num);


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


void Modulator_Init(void(*clb)(void))
{
	MsgSentClb = clb;

	DAC_init();

	PIT_Init();

	PIT_set_Timer(0, 1180, sendBit);  //Se usa una frecuencia más lenta que con el demodulador.

	PIT_Start_Timer(0);


	// Generacion de valores de señal de salida usando libreria math.
	for(int i = 0; i < SIN_VALUES; i++)
	{
		sinValues[i] = 2048*(1 + sin(2*3.141592*i/SIN_VALUES));
	}

	status = IDLE;
}




void Modulator_sendChar(char my_char)
{
	status = MSG;
	msg_ptr = 0;
	cont = 0;

	char msg = my_char;

	bitStream[0] = false; //START
	bitStream[STAND_LEN - 1] = true; //STOP
	bitStream[STAND_LEN - 2] = parity(msg);

	for(int i = 1; i < 9; i++)
	{
		bitStream[i] = (msg & TOBITMASK);
		msg = msg<<1;
	}


}

void procesBitStream(void)
{

	msg_ptr++;
	if(msg_ptr == STAND_LEN)
	{
		msg_ptr = 0;
		status = IDLE;
		MsgSentClb(); // Avisa que termino de mandar la señal modulada

	}


}


void sendBit(void)
{
	if(status == MSG)
	{
		if (bitStream[msg_ptr] == false) //si tengo que mandar 0 es 1.83 periodos
		{
			sendSin0();
		}
		else
		{
			sendSin1();
		}
	}
	else //IDLE
	{
		sendSin1();
	}

}

void sendSin1(void)
{
	DAC_SetData(sinValues[sinIndex]);

	sinIndex++;
	if(sinIndex == SIN_VALUES)
	{
		sinIndex = 0;
	}
	cont++;
	if(cont == SIN_VALUES)
	{
		cont = 0;
		if(status == MSG)
		{
			procesBitStream();
		}
	}
	else
	{
		// NO TERMINO DE TRANSMITIR
	}
}

void sendSin0(void)
{
	if(cont == 0)
	{
		aux = sinIndex;
	}

	DAC_SetData(sinValues[(uint8_t)aux]);

	aux+= ratio;
	cont++;

	if((uint8_t)aux >= SIN_VALUES)
	{
		aux = 0;
	}

	if(cont == 19)
	{
		cont = 0;
		cycle++;
	}

	if((cycle == 1) && (cont == 15)) // 1.83 ciclos para el 0
	{
		cycle = 0;
		cont = 0;
		if(status == MSG)
		{
			procesBitStream();
			sinIndex = (uint8_t)aux;
		}

	}
	else
	{
		// NO TERMINO DE TRANSMITIR
	}


}



bool parity (char num)
{
	bool par = 1;
	for (int i = 0 ; i < 7; i++)
	{
		par ^= ((num & (1<<i))>>i);
	}
	return par;
}

/*******************************************************************************
 ******************************************************************************/
