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
/*******************************************************************************
 * VARIABLES WITH LOCAL SCOPE
 ******************************************************************************/
static queue_uint16_t buffer;

static uint16_t ic_buffer[MAX_NUMBER_OF_STORED_SAMPLES];

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
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
	FTM_Init(FTM_3, FTM_PSC_x16, 0xFFFF, 0);
	FTM_CH_IC_Init(FTM_3, FTM_CH_5, FTM_IC_BOTH_EDGES, &IC_Fun);

	uint32_t * CnV_pointer = FTM_CH_GetCnVPointer(FTM_3, FTM_CH_5);

	DMA_source_t source_number = FTM3CH5;
	uint8_t channel = 1;

	uint32_t source_address = (uint32_t)CnV_pointer;
	uint32_t dest_address = (uint32_t) ic_buffer;
	uint8_t soff = 0;
	uint8_t doff = sizeof(ic_buffer[0]);
	uint8_t sSize = sizeof(ic_buffer[0]);
	uint32_t nbytes = sizeof(ic_buffer[0]);
	uint32_t citer = (sizeof(ic_buffer)) / (sizeof(ic_buffer[0]));
	uint32_t sourceBuffer_sizeof = 0;
	uint32_t destBuffer_sizeof = sizeof(ic_buffer);
	void (*cb) (void) = 0;

	dma0_init(source_number, channel, source_address, dest_address,
			soff, doff, sSize, nbytes,
			citer, sourceBuffer_sizeof, destBuffer_sizeof, cb);

}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

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
