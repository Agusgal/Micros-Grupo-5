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

#define	FTM_CYCLES_1200 	1302
#define	FTM_CYCLES_2200		710

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
	ERROR

};
/*******************************************************************************
 * VARIABLES WITH LOCAL SCOPE
 ******************************************************************************/
static queue_uint16_t buffer;

static queue_uint8_t out_buffer;

static uint16_t ic_buffer1[MAX_NUMBER_OF_STORED_SAMPLES];

static uint16_t ic_buffer2[MAX_NUMBER_OF_STORED_SAMPLES];

static uint16_t* buffer_pointer;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static void IC_Analysis (void);

static void IC_Fun (uint16_t CnV);

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


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void IC_Analysis (void)
{
	static uint8_t state = IDLE;
	uint16_t difBuffer[MAX_NUMBER_OF_STORED_SAMPLES - 1];
	uint16_t* prev_buffer_pointer = buffer_pointer;

	if(buffer_pointer == ic_buffer1)
	{
		buffer_pointer = ic_buffer2;
	}
	else if(buffer_pointer == ic_buffer2)
	{
		buffer_pointer = ic_buffer1;
	}
	dma0_disable(1);
	set_dma0_daddr(1, (uint32_t) buffer_pointer);
	dma0_enable(1);

	uint8_t i;
	for(i = 0; i < MAX_NUMBER_OF_STORED_SAMPLES - 1; i++)
	{
		difBuffer[i] = prev_buffer_pointer[i+1] - prev_buffer_pointer[i];
	}


	for(i = 0; i < MAX_NUMBER_OF_STORED_SAMPLES - 1; i++)
	{
		switch(state)
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
				state = ONE_DET_FLANK_OR_ZERO_STRONG;
			}

			break;

		// Cuando se detectó un "1" fuerte en el paso anterior
		case	ONE_STRONG_DETECTED:

			push_Queue_Element_uint8(out_buffer, 1);

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
			// Debería aparecer un "0" fuerte, sino, error.
			if(difBuffer[i] > H_THRESHOLD)
			{
				state = ERROR;
			}
			else if(difBuffer[i] < L_THRESHOLD)
			{
				// Guardamos el 0
				push_Queue_Element_uint8(out_buffer, 0);
				// Pasamos a saltear 2 LUGARES
				state = JUMP1;
			}
			else
			{
				state = ERROR;
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
				push_Queue_Element_uint8(out_buffer, 1);
				state = WAIT;
			}
			else if(difBuffer[i] < L_THRESHOLD)
			{
				// Guardamos el 0
				push_Queue_Element_uint8(out_buffer, 0);
				// Pasamos a saltear 2 LUGARES
				state = JUMP1;
			}
			else
			{
				state = ERROR;
			}
			break;

		case	ERROR:
			uint8_t error = 1;
			break;

		default:
			break;

		}
	}


}


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

/*******************************************************************************
 ******************************************************************************/
