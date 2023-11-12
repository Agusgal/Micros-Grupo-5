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
				  FTM3CH5 = 37,
                  DMADAC0 = 45,    DMADAC1 = 46, DMAALWAYS63 = 63} DMA_source_t;

/*******************************************************************************
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
		uint32_t citer, uint32_t sourceBuffer_sizeof, uint32_t destBuffer_sizeof, void (*cb) (void));

/**
 * @brief
 */
void set_dma0_source_offset(uint8_t channel, uint8_t soff);

/**
 * @brief
 */
void set_dma0_citer(uint8_t channel, uint32_t citer);

/**
 * @brief
 */
void set_dma0_biter(uint8_t channel, uint32_t biter);

/**
 * @brief
 */
void set_dma0_saddr(uint8_t channel, uint32_t source_address);

/**
 * @brief
 */
void set_dma0_slast(uint8_t channel, uint32_t slast);

/**
 * @brief
 */
uint32_t get_dma0_saddr(uint8_t channel);

/**
 * @brief
 */
void set_dma0_daddr(uint8_t channel, uint32_t destination_address);

/**
 * @brief
 */
void dma0_enable(uint8_t channel);

/**
 * @brief
 */
void dma0_disable(uint8_t channel);

#endif // _DMA_DRV_
