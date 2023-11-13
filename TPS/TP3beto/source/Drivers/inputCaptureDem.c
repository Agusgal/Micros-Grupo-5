/***************************************************************************//**
  @file     inputCaptureDem.h
  @brief    Application functions
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "inputCaptureDem.h"
#include "FTM.h"
#include "Queues/Queue.h"
#include "Drivers/dma_drv.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define	MAX_NUMBER_OF_STORED_SAMPLES	64

#define	FTM_CYCLES_1200 		2600

#define	FTM_CYCLES_1200_SEMI 	1302
#define	FTM_CYCLES_2200_SEMI	710

#define	L_THRESHOLD		910
#define	H_THRESHOLD		1102

enum
{
	FIRST_READ = 0,
	ONE_STRONG_DETECTED,
	ONE_DET_FLANK_OR_ZERO_STRONG,
	WAIT,
	JUMP1,
	JUMP2,
	FLANK_AFTER_WAIT,
	READING_CEROS,
	READING_UNOS,
	ERROR

};
/*******************************************************************************
 * VARIABLES WITH LOCAL SCOPE
 ******************************************************************************/
static queue_uint16_t buffer;

static queue_uint8_t out_buffer;

static queue_uint8_t uart_bytes_buffer;

static uint16_t ic_buffer1[MAX_NUMBER_OF_STORED_SAMPLES];

static uint16_t ic_buffer2[MAX_NUMBER_OF_STORED_SAMPLES];

static uint16_t* buffer_pointer;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static void IC_Analysis (void);

static void IC_Fun (uint16_t CnV);

static uint32_t round_uint32_division(uint32_t numerator, uint32_t denominator);

static void buildFrames(void);

static void uploadFrame(uint8_t * frame);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
/**
 * @brief
 */
void inputCaptureDem_init(void)
{
	queue_Init_uint16(&buffer);
	queue_Init_uint8(&out_buffer);
	queue_Init_uint8(&uart_bytes_buffer);

	FTM_Init(FTM_3, FTM_PSC_x16, 0xFFFF, 0);
	FTM_CH_IC_Init(FTM_3, FTM_CH_5, FTM_IC_BOTH_EDGES, &IC_Fun);
	FTM_CH_EnableDMA(FTM_3, FTM_CH_5);

	uint32_t * CnV_pointer = FTM_CH_GetCnVPointer(FTM_3, FTM_CH_5);

	DMA_source_t source_number = FTM3CH5;
	uint8_t channel = 1;

	buffer_pointer = ic_buffer1;

	uint32_t source_address = (uint32_t)CnV_pointer;
	uint32_t dest_address = (uint32_t) buffer_pointer;
	uint8_t soff = 0;
	uint8_t doff = sizeof(ic_buffer1[0]);
	uint8_t sSize = sizeof(ic_buffer1[0]);
	uint32_t nbytes = sizeof(ic_buffer1[0]);
	uint32_t citer = (sizeof(ic_buffer1)) / (sizeof(ic_buffer1[0]));
	uint32_t sourceBuffer_sizeof = 0;
	uint32_t destBuffer_sizeof = sizeof(ic_buffer1);
	void (*cb) (void) = IC_Analysis;

	dma0_init(source_number, channel, source_address, dest_address,
			soff, doff, sSize, nbytes,
			citer, sourceBuffer_sizeof, destBuffer_sizeof, cb);

}


/**
 * @brief
 * @param
 * @return
 */

uint8_t IC_DEM_Get_Status(void)
{
	return(get_Queue_Status_uint8(&uart_bytes_buffer));
}

/**
 * @brief
 * @param
 * @return
 */

uint8_t IC_DEM_Get_Data(void)
{
	return(pull_Queue_Element_uint8(&uart_bytes_buffer));
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void IC_Analysis (void)
{
	static uint8_t state = FIRST_READ;
	static uint32_t counter = 0;
	static uint8_t error = 0;
	static uint32_t sum = 0;
	static uint32_t hubo_flanco = 0;
	static uint8_t frame_index = 0;
	static uint32_t saved_numbers = 0;
	static uint32_t first_time = 1;

	uint16_t difBuffer[MAX_NUMBER_OF_STORED_SAMPLES - 1];
	uint16_t* prev_buffer_pointer = buffer_pointer;


	counter++;

	if(buffer_pointer == ic_buffer1)
	{
		buffer_pointer = ic_buffer2;
	}
	else if(buffer_pointer == ic_buffer2)
	{
		buffer_pointer = ic_buffer1;
	}

	uint16_t lastValue;
	if(first_time == 1)
	{
		lastValue = 0;
		first_time = 0;
	}
	else
	{
		lastValue = buffer_pointer[MAX_NUMBER_OF_STORED_SAMPLES-1];
	}


	dma0_disable(1);
	set_dma0_daddr(1, (uint32_t) buffer_pointer);
	dma0_enable(1);

	// Guardamos la diferencia de los ticks de FTM (CnV)
	difBuffer[0] = prev_buffer_pointer[0] - lastValue;

	uint8_t i;
	for(i = 0; i < MAX_NUMBER_OF_STORED_SAMPLES-1; i++)
	{
		difBuffer[i+1] = prev_buffer_pointer[i+1] - prev_buffer_pointer[i];
	}

	// Analizamos cómo interpretar la información (1200 y 2200 complicaron el proceso, por no ser múltiplos)
	for(i = 0; i < MAX_NUMBER_OF_STORED_SAMPLES; i++)
	{
		if(saved_numbers == 40)
		{
			uint8_t a = 0;
		}
		switch(state)
		{
		case	FIRST_READ:
			if(difBuffer[i] > H_THRESHOLD)
			{
				state = READING_UNOS;
				sum = difBuffer[i];
			}
			if(difBuffer[i] < H_THRESHOLD)
			{
				state = READING_CEROS;
				sum = difBuffer[i];
			}

			break;

		case	READING_CEROS:
			if(difBuffer[i] > H_THRESHOLD)
			{
				// Si leía 0s y ahora leí un 1 fuerte
				// Entonces, sum / FTM_CYCLES_1200
				// Incluso con un flanco inicial
				// Debería dar la cantidad de 0s leídos
				uint32_t cant = round_uint32_division(sum, FTM_CYCLES_1200);
				uint32_t j = 0;
				for(j = 0; j < cant; j++)
				{
					push_Queue_Element_uint8(&out_buffer, 0);
					saved_numbers++;
					saved_numbers = saved_numbers % 64;
				}
				hubo_flanco = 0;
				sum = difBuffer[i];
				state = READING_UNOS;
			}
			else if(difBuffer[i] < L_THRESHOLD)
			{
				sum += difBuffer[i];
			}
			else
			{
				if(hubo_flanco == 0)
				{
					sum += difBuffer[i];
				}

				uint32_t cant = round_uint32_division(sum, FTM_CYCLES_1200);
				uint32_t j = 0;
				for(j = 0; j < cant; j++)
				{
					push_Queue_Element_uint8(&out_buffer, 0);
					saved_numbers++;
					saved_numbers = saved_numbers % 64;
				}
				hubo_flanco = 1;
				sum = difBuffer[i];
				state = READING_UNOS;
			}

			break;

		case 	READING_UNOS:
			if(difBuffer[i] > H_THRESHOLD)
			{
				sum += difBuffer[i];
				if(sum >= FTM_CYCLES_1200 * 11)
				{
					uint8_t j;
					for(j = 0; j < 11; j++)
					{
						push_Queue_Element_uint8(&out_buffer, 1);
						saved_numbers++;
						saved_numbers = saved_numbers % 64;
					}

					sum = 0;
				}
			}
			else if(difBuffer[i] < L_THRESHOLD)
			{
				// Si leía 0s y ahora leí un 1 fuerte
				// Entonces, sum / FTM_CYCLES_1200
				// Incluso con un flanco inicial
				// Debería dar la cantidad de 0s leídos
				uint32_t cant = round_uint32_division(sum, FTM_CYCLES_1200);
				uint32_t j = 0;
				for(j = 0; j < cant; j++)
				{
					push_Queue_Element_uint8(&out_buffer, 1);
					saved_numbers++;
					saved_numbers = saved_numbers % 64;
				}
				hubo_flanco = 0;
				sum = difBuffer[i];
				state = READING_CEROS;
			}
			else
			{
				// Si leía 1s y ahora leí un 0 fuerte
				// Entonces, sum / FTM_CYCLES_1200
				// Incluso con un flanco inicial
				// Debería dar la cantidad de 1s leídos
				if(hubo_flanco == 0)
				{
					sum += difBuffer[i];
				}
				uint32_t cant = round_uint32_division(sum, FTM_CYCLES_1200);
				uint32_t j = 0;
				for(j = 0; j < cant; j++)
				{
					push_Queue_Element_uint8(&out_buffer, 1);
					saved_numbers++;
					saved_numbers = saved_numbers % 64;
				}
				hubo_flanco = 1;
				sum = difBuffer[i];
				state = READING_CEROS;
			}
			break;
		}
	}

	buildFrames();

}

static void buildFrames(void)
{
	static uint8_t frame[11];
	static uint8_t frame_index = 0;
	uint8_t element;

	while(get_Queue_Status_uint8(&out_buffer))
	{
		element = pull_Queue_Element_uint8(&out_buffer);
		if(frame_index == 0 && element == 0)
		{
			frame[0] = element;
			frame_index++;
		}
		else if(frame_index < 10 && frame_index > 0)
		{
			frame[frame_index] = element;
			frame_index++;
		}
		else if(frame_index == 10)
		{
			frame[frame_index] = element;
			uploadFrame(frame);
			frame_index = 0;
		}
	}
}

static void uploadFrame(uint8_t * frame)
{
	uint8_t i = 0;
	uint8_t result = 0;
	for(i = 1; i < 9; i++)
	{
		result |= frame[i] << (i-1);
	}
	push_Queue_Element_uint8(&uart_bytes_buffer, result);
}

///////////////////////////////////////////////////
static void IC_Fun (uint16_t CnV)
{
	static uint16_t med1,med2;
	static uint8_t  state=0;

	if(state==0)
	{
		med1=CnV;
		push_Queue_Element_uint16(&buffer, med1 - med2);
		state=1;
	}
	else if(state==1)
	{
		med2=CnV;
		push_Queue_Element_uint16(&buffer, med2 - med1);
		state=0;					// Set break point here and watch "med" value
	}
}

/**
 * @brief
 */
static uint32_t round_uint32_division(uint32_t numerator, uint32_t denominator)
{
    if (denominator == 0)
    {
        // Handle division by zero
        return 0;  // You can choose an appropriate value or error handling here
    }

    uint32_t result = numerator / denominator;
    uint32_t remainder = numerator % denominator;

    // Round to the nearest integer
    if (2 * remainder >= denominator)
    {
        result++;
    }

    return result;
}
/*******************************************************************************
 ******************************************************************************/

/*switch(state)
{
// En el INIT, el algoritmo se sincroniza con un "1" de IDLE, el resto debería andar a partir de esto
case	FIRST_READ:
	if(difBuffer[i] > H_THRESHOLD)
	{
		state = ONE_STRONG_DETECTED;
	}
	break;

// Cuando ya se realizaron los salteos, y espero que venga lo que tenga que venir.
case	WAIT:
	// Si viene un 1 fuerte, paso a ver el suguiente
	if(difBuffer[i] > H_THRESHOLD)
	{
		state = ONE_STRONG_DETECTED;
	}
	// Si viene un 0, paso a ver el suiente para verificar el 0
	else if(difBuffer[i] < L_THRESHOLD)
	{
		state = ONE_DET_FLANK_OR_ZERO_STRONG;
	}
	else
	{
		state = FLANK_AFTER_WAIT;
	}

	break;

// Cuando se detectó un "1" fuerte en el paso anterior
case	ONE_STRONG_DETECTED:

	push_Queue_Element_uint8(&out_buffer, 1);

	// Si hay otro "1" fuerte, salteo y espero a lo que venga
	if(difBuffer[i] > H_THRESHOLD)
	{
		state = WAIT;
	}
	// Si hay otro un "flanco" o un "0" fuerte, paso a leer el siguiente para confirmar
	else if(difBuffer[i] < L_THRESHOLD)
	{
		state = ONE_DET_FLANK_OR_ZERO_STRONG;
	}
	else
	{
		state = ONE_DET_FLANK_OR_ZERO_STRONG;
	}

	break;

// Si se detectó un "1", y después vino un "flanco" o un "0" fuerte.
// Ó, se detectó directamente "0" fuerte después de un WAIT
case ONE_DET_FLANK_OR_ZERO_STRONG:
	// Debería aparecer un "0" fuerte, sino, lo ignoro
	// Ocurriría si hay un error de sinc por "0"s consecutivos
	if(difBuffer[i] > H_THRESHOLD)
	{
		state = ONE_STRONG_DETECTED;
	}
	else if(difBuffer[i] < L_THRESHOLD)
	{
		// Guardamos el 0
		push_Queue_Element_uint8(&out_buffer, 0);
		// Pasamos a saltear 2 LUGARES
		state = JUMP1;
	}
	else
	{
		state = WAIT; // Ocurriría si hay un error de sinc por "0"s consecutivos
	}


	break;

case JUMP1:
	state = JUMP2;
	break;

case JUMP2:
	// Una vez saltado, pasamos a esperar
	state = WAIT;
	break;

// Si después de un WAIT, se detecta un flanco, se lee el siguiente para decidir
case	FLANK_AFTER_WAIT:
	// Si es un "1" fuerte, lo guardamos y pasamos a WAIT
	if(difBuffer[i] > H_THRESHOLD)
	{
		push_Queue_Element_uint8(&out_buffer, 1);
		state = WAIT;
	}
	else if(difBuffer[i] < L_THRESHOLD)
	{
		// Guardamos el 0
		push_Queue_Element_uint8(&out_buffer, 0);
		// Pasamos a saltear 2 LUGARES
		state = JUMP1;
	}
	else
	{
		state = ERROR;
	}
	break;

case	ERROR:
	error = 1;
	break;

default:
	break;

}*/
