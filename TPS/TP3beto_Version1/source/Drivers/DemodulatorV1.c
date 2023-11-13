/*
 * DemodulatorV1
 *
 *  Created on: Nov. 2023
 *      Author: Grupo 5
 */

#include "DemodulatorV1.h"
#include "ADC.h"
#include "PIT.h"


#define ADC_CH 0
#define MSG_LEN 11


typedef enum {IDLE, MSG}status_t;

void filterSignal (void);
void updateData();


//Data recien salida del adc
int16_t rawData[DELAY + 1] = {0};  //DELAY+1

//Data antes de ser filtrada
float m_t[FIR_ORDER + 1] = {0};

//Señal a la salida del filtro, luego cuento 1s y 0s y determino nivel reald e la señal de UART
bool FilterOutput[10];

//Mensjae que el callback le pasa a la aplicacion cuando completo el parseo.
static bool bitstream[11];

//Variables para determianr casos limites de primeras muestras.
bool value = true;
bool firstZero = true;

//Señala si los datos salieron del ADC y por ende se puede procesar.
static bool ADCoutputready = false;


//Callback que se ejecuta cuando debo pasar el mensaje a la aplicacion.
static myCallback funcioncallback = NULL;


int outputCount = 0;
int sampleCount = 0;

static status_t status = IDLE;


static float firCoef[ ] =
		{-0.0018693054003477631f,
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
		-0.0018693054003477631f };


void Demodulator_Init(myCallback clb)
{
	funcioncallback = clb;
	ADC_Init(ADC_CH, updateData);
	ADC_EnableModule(ADC_CH);

	PIT_set_Timer(1, 4167, ADC_StartConvertion);   //4166 es 12kHz en tiempos de 20ns.
	PIT_Start_Timer(1);
}


void demodulate()
{
	ADCoutputready = false;

	uint8_t sum = 0;

	//Shifteo para poder hacer entrar el siguiente dato.
	for(int i = 1; i < (1+ FIR_ORDER); i++)
	{
		m_t[i - 1] = m_t[i];
	}

	//Actualizo ultimo valor
	m_t[FIR_ORDER] = rawData[DELAY] * rawData[0];

	//Filtro la señal en el FIR
	filterSignal();

	sampleCount += 1;

	//Este codigo es el comparador, cuenta unos y ceros paa determinar el nivle de la señal final.
	if (status == MSG)
	{
		if (sampleCount == 10)
		{
			sampleCount = 0;
			for(int j = 0; j < 10 ; j++)
			{
				sum += FilterOutput[j];
			}
			if (sum > 5)
			{
				bitstream[outputCount] = true;
			}
			else
			{
				bitstream[outputCount] = false;
			}

			//Cuando llego al largo de la palabra de UART ejecuto el callback para pasar el mensaje a la App.
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
			for(int j = 0; j < 10 ; j++)
			{
				sum += FilterOutput[j];
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
					bitstream[0] = false;
				}
				else
				{
					firstZero = false; //el primero
				}

			}
			else
			{
				sampleCount = 9;
				for(int i = 1; i < 10; i++)
				{
					FilterOutput[i - 1] = FilterOutput[i];
				}

			}
		}
	}
}

void updateData(void)
{
	uint16_t newdata = ADC_getData();

	for(int i = 1; i < (1 + DELAY); i++)
	{
		rawData[i - 1] = rawData[i];
	}

	rawData[DELAY] = newdata - 2048; //deberia ser 4055??
	ADCoutputready = true;
}

void filterSignal(void)
{
	float aux = 0;

	for (int k = 0; k <= FIR_ORDER ; k++ )
	{
		aux += firCoef[k] * m_t[FIR_ORDER - k];
	}

	if (aux < 0)
	{
		FilterOutput[sampleCount] = true;
	}
	else
	{
		FilterOutput[sampleCount] = false;
	}

}
//todo; revisar si hayq eud ar vuelta esto.
char get_Msg(void)
{
	char retmsg = 0;

	for (int k = 1; k < 9;k++ )
	{
		retmsg |= (bitstream[k]<< (8 - k));
	}

	return retmsg;
}

bool isDataReady(void)
{
	return ADCoutputready;
}
