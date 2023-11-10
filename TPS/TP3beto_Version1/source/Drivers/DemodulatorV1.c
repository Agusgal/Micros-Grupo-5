/*
 * DemodulatorV1.c
 *
 *  Created on: 10 nov. 2023
 *      Author: agus
 */




#include "DemodulatorV1.h"
#include "ADC.h"
#include "PIT.h"

//Uso canal 0 del ADC
#define ADC_CH 0

//Largo de palabra
#define MSG_LEN 11

//Una mini maquina de estados, determino si estoy recibiendo o en idle
typedef enum {IDLE, MSG}status_t;


void filterSignal (void);
void updateData();



int16_t rawData [DELAY + 1] = {0};   //DELAY+1
float prefilter[FIR_ORDER+1] = {0};
bool output[10];
static bool msg[11];
bool value = true;
bool firstZero = true;
static bool dataReady = false;


static myCallback funcioncallback = NULL;


int outputCount = 0;
int sampleCount = 0;

static status_t status = IDLE;


//Filtros cortesia de Newtonis
static float firCoef[ ] =
		{
		-0.0018693054003477631f,
		-0.0063458560700613868f,
		-0.011803262238256836f,
		-0.011431063027960632f,
		 0.0055142394274450349f,
		 0.047971856405729658f,
		 0.11520334422221702f,
		 0.1924527660178473f,
		 0.2548404235990876f,
		 0.27884873508664521f,
		 0.2548404235990876f,
		 0.1924527660178473f,
		 0.11520334422221702f,
		 0.047971856405729658f,
		 0.0055142394274450349f,
		-0.011431063027960632f,
		-0.011803262238256836f,
		-0.0063458560700613868f,
		-0.0018693054003477631f
		};

void Demodulator_Init(myCallback funcallback)
{

	funcioncallback = funcallback;
	ADC_Init(ADC_CH, updateData);
	ADC_EnableModule(ADC_CH);


	PIT_set_Timer(2, 4167, ADC_startConvertion);  //4166 en saltos de 20ns es 12kHz
	PIT_Start_Timer(2);
}


void demodulate()
{
	dataReady = false;
	//updateData();

	uint8_t sum = 0;

	for(int i=1; i < (1+ FIR_ORDER); i++)
	{
		prefilter[i-1] = prefilter[i];
	}

	prefilter[FIR_ORDER] = rawData[DELAY]*rawData[0];
	filterSignal();
	sampleCount = sampleCount + 1;


	if (status == MSG)
	{
		if (sampleCount == 10)
		{
			sampleCount = 0;
			for(int j = 0; j<10 ; j++)
			{
				sum+= output[j];
			}
			if (sum > 5)
			{
				msg[outputCount] = true;
			}
			else
			{
				msg[outputCount] = false;
			}

			outputCount = outputCount + 1;
			if (outputCount == MSG_LEN)
			{
				funcioncallback();
				status = IDLE;
			}


		}
	}
	else
	{
		if (sampleCount == 10)
		{
			sampleCount = 0;
			for(int j = 0; j<10 ; j++)
			{
				sum+= output[j];
			}
 			if (sum >= 5)
			{
				value = true;
			}
			else
			{
				value = false;
			}

			if ((value == false))
			{
				if((firstZero == false))
				{
					outputCount = 1;
					sampleCount = 0;
					status = MSG;
					msg[0] = false;
				}
				else
				{
					firstZero = false; //el primer
				}

			}
			else
			{
				sampleCount = 9;
				for(int i=1; i < 10; i++)
				{
					output[i-1] = output[i];
				}

			}
		}
	}
}

void updateData(void)
{
	uint16_t newdata = ADC_getDataResult();

	for(int i=1; i < (1+ DELAY); i++)
	{
		rawData[i-1] = rawData[i];
	}
	rawData[DELAY] = newdata - 2048;
	dataReady = true;

}

void filterSignal(void)
{
	float aux = 0;

	for (int k = 0; k <= FIR_ORDER ; k++ )
	{
		aux += firCoef[k] * prefilter[FIR_ORDER - k];
	}

	if (aux < 0)
	{
		output[sampleCount] = true;
	}
	else
	{
		output[sampleCount] = false;
	}

}

char get_Msg(void)
{
	char retmsg = 0;

	for (int k = 1; k < 9;k++ )
	{
		retmsg |= (msg[k]<< (8 - k));
	}

	return retmsg;
}

bool isDataReady(void)
{
	return dataReady;
}
