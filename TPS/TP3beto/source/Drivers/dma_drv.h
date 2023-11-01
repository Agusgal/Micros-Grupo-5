/***************************************************************************//**
  @file     DMA_DRV.h
  @brief    Board management
  @author   grupo 5
 ******************************************************************************/

#ifndef _DMA_DRV_
#define _DMA_DRV_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

typedef enum {    FTM0CH0 = 20,     FTM0CH1 = 21, FTM0CH2 = 22, FTM0CH3 = 23,
                  FTM0CH4 = 24,     FTM0CH5 = 25, FTM0CH6 = 26, FTM0CH7 = 27,
                  DMADAC0 = 45,    DMADAC1 = 46, DMAALWAYS63 = 63} DMA_source_t;

/*******************************************************************************
 ******************************************************************************/

/**
* @brief Initiates a DMA channel with the configuration provided
*
* @param source_number			Source that generates the DMA request
* @param channel				Selects DMA channel
* @param source_address			Address of the source of the data
* @param dest_address			Address of the destination of the data
* @param soff					Source offset
* @param doff					Destination offset
* @param sSize					Size of each data (number of bytes) transfer at source (should be equal to dSize)
* @param nbytes					Number of bytes to be transfered every DMA request (should be multiple of sSize)
* @param citer					Number of major loop cycles.
* @param sourceBuffer_sizeof	Number of bytes of source buffer (sizeof(sBuffer))
*/
void dma0_init(DMA_source_t source_number, uint8_t channel, uint32_t * source_address, uint32_t * dest_address,
		uint8_t soff, uint8_t doff, uint8_t sSize, uint32_t nbytes,
		uint32_t citer, uint32_t sourceBuffer_sizeof, uint32_t destBuffer_sizeof);

#endif // _DMA_DRV_
