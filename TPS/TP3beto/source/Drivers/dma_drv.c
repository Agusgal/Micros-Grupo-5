/***************************************************************************//**
  @file     DMA_DRV.C
  @brief    Application functions
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "dma_drv.h"
#include "MK64F12.h"
#include "hardware.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define NUMBER_OF_CHANNELS	16

/*******************************************************************************
 * VARIABLES WITH LOCAL SCOPE
 ******************************************************************************/
void (*callback[NUMBER_OF_CHANNELS]) (void);


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/**
 * @brief Initiates a DMA channel with the configuration provided
 *
 * @param source_number			Source that generates the DMA request
 * @param channel				Selects DMA channel
 * @param source_address		Address of the source of the data (in uint32_t)
 * @param dest_address			Address of the destination of the data (in uint32_t)
 * @param soff					Source offset
 * @param doff					Destination offset
 * @param sSize					Size of each data (number of bytes) transfer at source (should be equal to dSize)
 * @param nbytes				Number of bytes to be transfered every DMA request (should be multiple of sSize)
 * @param citer					Number of major loop cycles.
 * @param sourceBuffer_sizeof	Number of bytes of source buffer (sizeof(sBuffer))
 * @param destBuffer_sizeof		Number of bytes of dest buffer (sizeof(dBuffer))
 * @param cb					Callback function to be called
 */
void dma0_init(DMA_source_t source_number, uint8_t channel, uint32_t source_address, uint32_t dest_address,
			uint8_t soff, uint8_t doff, uint8_t sSize, uint32_t nbytes,
			uint32_t citer, uint32_t sourceBuffer_sizeof, uint32_t destBuffer_sizeof, void (*cb) (void))
{

	if(cb)
	{
		callback[channel] = cb;
	}

	/* Enable the clock for the eDMA and the DMAMUX. */
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;

	/* Enable the eDMA channel 0 and set the FTM CH0 as the DMA request source. */
	DMAMUX->CHCFG[channel] |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(source_number) | DMAMUX_CHCFG_TRIG(0);   // FTM0 CH0 == 20

	/* Enable the interrupts for the channel 0. */

	/* Clear all the pending events. */
	NVIC_ClearPendingIRQ(DMA0_IRQn);
	/* Enable the DMA interrupts. */
	NVIC_EnableIRQ(DMA0_IRQn);


	/// ============= INIT TCD0 ===================//
	//DMA0->TCD[channel].CSR &= ~DMA_CSR_ESG(0);								// No Scatter and Gather

	/* Set memory address for source and destination. */
	DMA0->TCD[channel].SADDR = (uint32_t)(source_address);				   //List of Duties

	/* Set memory address for source and destination. */
	DMA0->TCD[channel].DADDR = (uint32_t)(dest_address);				   //List of Duties


	/* Set an offset for source and destination address. */
	DMA0->TCD[channel].SOFF = soff; // Source address offset of 2 bytes per transaction.
	DMA0->TCD[channel].DOFF = doff; // Destination address offset is 0. (Siempre al mismo lugar)

	/* Set source and destination data transfer size to 16 bits (CnV is 2 bytes wide). */
	if(sSize == 1 || sSize == 2 || sSize == 4)
	{
		DMA0->TCD[channel].ATTR = DMA_ATTR_SSIZE(sSize >> 1) | DMA_ATTR_DSIZE(sSize >> 1);
	}
	else
	{
		DMA0->TCD[channel].ATTR = DMA_ATTR_SSIZE(0) | DMA_ATTR_DSIZE(0);
	}


	/*Number of bytes to be transfered in each service request of the channel.*/
	DMA0->TCD[channel].NBYTES_MLNO = nbytes;

	/* Current major iteration count. */

	/* Autosize for Current major iteration count */
	DMA0->TCD[channel].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(citer);
	DMA0->TCD[channel].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(citer);


	/* Autosize SLAST for Wrap Around. This value is added to SADD at the end of Major Loop */
	DMA0->TCD[channel].SLAST = -sourceBuffer_sizeof;


    /* DLASTSGA DLAST Scatter and Gatter */
    DMA0->TCD[channel].DLAST_SGA = -destBuffer_sizeof;

	/* Setup control and status register. */

	DMA0->TCD[channel].CSR = DMA_CSR_INTMAJOR_MASK;	//Enable Major Interrupt.

	DMA0->EEI |= DMA_EEI_EEI0(1); //Enable error interrupt

	/* Enable request signal for channel 0. */
	DMA0->ERQ |= 1 << channel;

}

/**
 * @brief
 */
void set_dma0_source_offset(uint8_t channel, uint8_t soff)
{
	DMA0->TCD[channel].SOFF = soff;
}

/**
 * @brief
 */
void set_dma0_citer(uint8_t channel, uint32_t citer)
{
	DMA0->TCD[channel].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(citer);
}

/**
 * @brief
 */
void set_dma0_biter(uint8_t channel, uint32_t biter)
{
	DMA0->TCD[channel].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(biter);
}

/**
 * @brief
 */
void set_dma0_saddr(uint8_t channel, uint32_t source_address)
{
	DMA0->TCD[channel].SADDR = (uint32_t)(source_address);
}

/**
 * @brief
 */
void set_dma0_daddr(uint8_t channel, uint32_t destination_address)
{
	DMA0->TCD[channel].DADDR = (uint32_t)(destination_address);
}

/**
 * @brief
 */
void set_dma0_slast(uint8_t channel, uint32_t slast)
{
	DMA0->TCD[channel].SLAST = -slast;
}

/**
 * @brief
 */
uint32_t get_dma0_saddr(uint8_t channel)
{
	return DMA0->TCD[channel].SADDR;
}


/**
 * @brief
 */
void dma0_enable(uint8_t channel)
{
	/* Enable request signal for channel 0. */
	DMA0->ERQ |= 1 << channel;
	DMAMUX->CHCFG[channel] |= DMAMUX_CHCFG_ENBL_MASK;
}

/**
 * @brief
 */
void dma0_disable(uint8_t channel)
{
	DMA0->ERQ &= ~(1 << channel);
	DMAMUX->CHCFG[channel] &= ~(DMAMUX_CHCFG_ENBL_MASK);
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void DMA0_IRQHandler(void)
{
	uint32_t status;
	//DMA0->CINT |= 0;
	status = DMA0->INT;
	DMA0->INT |= 0xFFFF;	//clear flags

	uint8_t i = 0;
	for(i = 0; i < NUMBER_OF_CHANNELS; i++)
	{
		if(status & (1 << i))
		{
			if(callback[i])
			{
				callback[i]();
			}
		}
	}

}

void DMA_Error_IRQHandler()
{
	static uint32_t b = 0;
	/* Clear the error interrupt flag.*/
	DMA0->CERR |= 0;

	b++;

}


/*******************************************************************************
 ******************************************************************************/
