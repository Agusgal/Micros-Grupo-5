/***************************************************************************//**
  @file     DMA_DRV.C
  @brief    Application functions
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "dma_drv.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/**
 * @brief
 */
void dma_init(uint8_t source_number, uint8_t channel, uint32_t * source_address, uint32_t * dest_address,
			uint8_t soff, uint8_t doff, uint8_t sSize, uint8_t dSize)
{
	/* Enable the clock for the eDMA and the DMAMUX. */
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;

	/* Enable the eDMA channel 0 and set the FTM CH0 as the DMA request source. */
	DMAMUX->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(source_number);   // FTM0 CH0 == 20

	/* Enable the interrupts for the channel 0. */

	/* Clear all the pending events. */
	NVIC_ClearPendingIRQ(DMA0_IRQn);
	/* Enable the DMA interrupts. */
	NVIC_EnableIRQ(DMA0_IRQn);


	/// ============= INIT TCD0 ===================//
	/* Set memory address for source and destination. */
	DMA0->TCD[channel].SADDR = (uint32_t)(source_address);				   //List of Duties

	/* Set memory address for source and destination. */
	DMA0->TCD[channel].DADDR = (uint32_t)(dest_address);				   //List of Duties


	/* Set an offset for source and destination address. */
	DMA0->TCD[channel].SOFF = soff; // Source address offset of 2 bytes per transaction.
	DMA0->TCD[channel].DOFF = doff; // Destination address offset is 0. (Siempre al mismo lugar)

	/* Set source and destination data transfer size to 16 bits (CnV is 2 bytes wide). */
	DMA0->TCD[channel].ATTR = DMA_ATTR_SSIZE(sSize) | DMA_ATTR_DSIZE(dSize);

	/*Number of bytes to be transfered in each service request of the channel.*/
	DMA0->TCD[0].NBYTES_MLNO = 0x02;

	/* Current major iteration count (5 iteration of 1 byte each one). */

	//DMA_TCD0_CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(0x05);
	//DMA_TCD0_BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(0x05);

	/* Autosize for Current major iteration count */
	DMA0->TCD[0].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER((sizeof(sourceBuffer)/sizeof(sourceBuffer[0])));
	DMA0->TCD[0].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER((sizeof(sourceBuffer)/sizeof(sourceBuffer[0])));

}



/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/




/*******************************************************************************
 ******************************************************************************/
