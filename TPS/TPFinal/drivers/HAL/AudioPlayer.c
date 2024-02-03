/***************************************************************************//**
 @file     AudioPlayer.c
 @brief    Audio Reproductor using the DAC with PDB.
 @author   Grupo 5
******************************************************************************/


/*
 * Based on the SDK example of DAC with PDB:
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/*******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include "AudioPlayer.h"
#include "fsl_dac.h"
#include "fsl_edma.h"
#include "fsl_pdb.h"
#include "fsl_dmamux.h"
#include "fsl_common.h"


#include "ftm.h"
#include "board.h"
#include "MK64F12.h"



/*******************************************************************************
* CONSTANT AND MACRO DEFINITIONS USING #DEFINE
******************************************************************************/
#define PDB_BASEADDR           PDB0
#define PDB_MODULUS_VALUE      0xFFFU
#define PDB_DELAY_VALUE        0U
#define PDB_DAC_CHANNEL        kPDB_DACTriggerChannel0
#define PDB_DAC_INTERVAL_VALUE 0xFFFU
#define DAC_BASEADDR           DAC0
#define DMAMUX_BASEADDR        DMAMUX
#define DMA_CHANNEL            1U
#define DMA_DAC_SOURCE         45U
#define DMA_BASEADDR           DMA0
#define DAC_DATA_REG_ADDR      0x400cc000U
#define DMA_IRQ_ID             DMA0_IRQn

#define DMA_CHANEL (0U)

/*******************************************************************************
* FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
******************************************************************************/
/*!
 * @brief Initialize the EDMA.
 */
static void EDMA_Configuration(void);


/*!
 * @brief Initialize the DMAMUX.
 */
static void DMAMUX_Configuration(void);


/*!
 * @brief Initialize the PDB.
 */
static void PDB_Configuration(void);


/*!
 * @brief Initialize the DAC.
 */
static void DAC_Configuration(void);


/*!
 * @brief Callback function for EDMA.
 */
static void Edma_Callback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds);


/*!
 * @brief Performs the division between two unsigned ints, and returns the rounded result
 */
static uint32_t roundedDivide(uint32_t dividend, uint32_t divisor);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static edma_handle_t audioPlayer_EDMA_Handle;                             /* Edma handler */
static edma_transfer_config_t audioPlayer_transferConfig;                 /* Edma transfer config. */
static volatile uint32_t sampleIndex = 0; 							/* Index of the g_dacDataArray array. */

// Buffers
static int16_t buffers[2][AUDIO_PLAYER_BUFF_SIZE];
static int16_t * activeBuffer = buffers[0];					// Active has the current playing sound
static int16_t * backBuffer= buffers[1];					// Back has the next frame of sound to be played

static uint32_t backBufferSampleRate;

static bool backBufferFree = false;
static bool pause = false;

static int16_t muteAudioBuffer[DAC_DATL_COUNT] = {DAC_ZERO_VOLT_VALUE};


/*******************************************************************************
*******************************************************************************
                       GLOBAL FUNCTION DEFINITIONS
*******************************************************************************
******************************************************************************/
void AudioPlayer_Init(void)
{
	for(uint8_t i = 0; i < DAC_DATL_COUNT; i++)
	{
		muteAudioBuffer[i] = DAC_ZERO_VOLT_VALUE;
	}

	// Initialize DMAMUX.
	DMAMUX_Configuration();

	// Initialize EDMA.
	EDMA_Configuration();

	// Initialize the HW trigger source.
	PDB_Configuration();

	// Initialize DAC.
	DAC_Configuration();
}


void AudioPlayer_LoadSong(uint16_t * firstSongFrame, uint16_t _sampleRate)
{
	memcpy(activeBuffer, firstSongFrame, AUDIO_PLAYER_BUFF_SIZE * sizeof(uint16_t));

	backBufferFree = true;
	sampleIndex = 0;

	backBufferSampleRate = _sampleRate;

	AudioPlayer_UpdateSampleRate(backBufferSampleRate);
}


void AudioPlayer_setSampleRate(uint32_t _sampleRate)
{
	backBufferSampleRate = _sampleRate;
}


void AudioPlayer_UpdateSampleRate(uint32_t _sampleRate)
{
	// PDB_Configuration
    pdb_config_t pdbConfigStruct;
    pdb_dac_trigger_config_t pdbDacTriggerConfigStruct;
    PDB_GetDefaultConfig(&pdbConfigStruct);

    /*
	else //For testing with DEMO case
	{
		pdbConfigStruct.dividerMultiplicationFactor = kPDB_DividerMultiplicationFactor40;
		pdbConfigStruct.prescalerDivider = kPDB_PrescalerDivider1;
		PDB_SetModulusValue(PDB_BASEADDR, PDB_MODULUS_VALUE);
	}*/

	pdbConfigStruct.dividerMultiplicationFactor = kPDB_DividerMultiplicationFactor1;
	pdbConfigStruct.prescalerDivider = kPDB_PrescalerDivider1;

	// Perform the required division
	PDB_SetModulusValue(PDB_BASEADDR, roundedDivide(60000000UL, _sampleRate));
	PDB_SetDACTriggerIntervalValue(PDB_BASEADDR, PDB_DAC_CHANNEL, roundedDivide(60000000UL, _sampleRate));

	pdbConfigStruct.enableContinuousMode = true;

	PDB_Init(PDB_BASEADDR, &pdbConfigStruct);
	PDB_EnableInterrupts(PDB_BASEADDR, kPDB_DelayInterruptEnable);
	PDB_SetCounterDelayValue(PDB_BASEADDR, PDB_DELAY_VALUE);

    // Set DAC trigger
    pdbDacTriggerConfigStruct.enableExternalTriggerInput = false;
    pdbDacTriggerConfigStruct.enableIntervalTrigger      = true;
    PDB_SetDACTriggerConfig(PDB_BASEADDR, PDB_DAC_CHANNEL, &pdbDacTriggerConfigStruct);

    // Load PDB values
    PDB_DoLoadValues(PDB_BASEADDR);
}


audioPlayerError_t AudioPlayer_UpdateBackBuffer(uint16_t * newBackBuffer, uint32_t _sampleRate)
{
	if(backBufferFree)
	{
		memcpy(backBuffer, newBackBuffer, AUDIO_PLAYER_BUFF_SIZE * sizeof(uint16_t));
		backBufferFree = false;

		// Update the backBuffer
		backBufferSampleRate = _sampleRate;
		return AP_NO_ERROR;
	}
	else
	{
		return AP_ERROR_BB_NOT_FREE;
	}
}


bool AudioPlayer_IsBackBufferFree(void)
{
	return backBufferFree;
}


void AudioPlayer_Play(void)
{
	sampleIndex = 0;

	if(pause)
	{
		pause = false;
		return;
	}

	// DMAMUX:
    DMAMUX_EnableChannel(DMAMUX_BASEADDR, DMA_CHANNEL);

	// EDMA:
    edma_transfer_config_t * config = &audioPlayer_transferConfig;

    // Source
    void *srcAddr = (void *)(activeBuffer + sampleIndex);
    uint32_t srcWidth = sizeof(uint16_t);

    // Address
    void *destAddr = (void *)DAC_DATA_REG_ADDR;
    uint32_t destWidth =  sizeof(uint16_t);
    uint32_t bytesEachRequest = DAC_DATL_COUNT * sizeof(uint16_t);
    uint32_t transferBytes = DAC_DATL_COUNT * sizeof(uint16_t);
    edma_transfer_type_t type = kEDMA_MemoryToMemory;

    EDMA_PrepareTransfer(config,
						  srcAddr,
						  srcWidth,
						  destAddr,
						  destWidth,
						  bytesEachRequest,
						  transferBytes,
						  type);

	EDMA_SubmitTransfer(&audioPlayer_EDMA_Handle, &audioPlayer_transferConfig);

	// Enable interrupt when transfer is done.
	EDMA_EnableChannelInterrupts(DMA_BASEADDR, DMA_CHANNEL, kEDMA_MajorInterruptEnable);

	// Enable transfer.
	EDMA_StartTransfer(&audioPlayer_EDMA_Handle);

	// DAC:
	// Enable DMA.
	DAC_EnableBufferInterrupts(DAC_BASEADDR, kDAC_BufferReadPointerTopInterruptEnable);
	DAC_EnableBufferDMA(DAC_BASEADDR, true);

	PDB_DoSoftwareTrigger(PDB_BASEADDR);
}


void AudioPlayer_Pause(void)
{
	pause = true;
}


/*******************************************************************************
*******************************************************************************
                       LOCAL FUNCTION DEFINITIONS
*******************************************************************************
******************************************************************************/
static void EDMA_Configuration(void)
{
	EDMA_CreateHandle(&audioPlayer_EDMA_Handle, DMA_BASEADDR, DMA_CHANNEL);
    EDMA_SetCallback(&audioPlayer_EDMA_Handle, Edma_Callback, NULL);
}


static void DMAMUX_Configuration(void)
{
    // Configure DMAMUX
    // Init en matrix // DMAMUX_Init(DMAMUX_BASEADDR);
    DMAMUX_SetSource(DMAMUX_BASEADDR, DMA_CHANNEL, DMA_DAC_SOURCE);
}


static void PDB_Configuration(void)
{
	// Enable the trigger source of PDB.
    pdb_config_t pdbConfigStruct;
    pdb_dac_trigger_config_t pdbDacTriggerConfigStruct;

    PDB_GetDefaultConfig(&pdbConfigStruct);

    // Paso esto a load song info para cambiar los valores para el PDB
    pdbConfigStruct.dividerMultiplicationFactor = kPDB_DividerMultiplicationFactor40;
    pdbConfigStruct.enableContinuousMode = true;

    PDB_Init(PDB_BASEADDR, &pdbConfigStruct);
    PDB_EnableInterrupts(PDB_BASEADDR, kPDB_DelayInterruptEnable);
    PDB_SetModulusValue(PDB_BASEADDR, PDB_MODULUS_VALUE);
    PDB_SetCounterDelayValue(PDB_BASEADDR, PDB_DELAY_VALUE);

    // Set DAC trigger.
    pdbDacTriggerConfigStruct.enableExternalTriggerInput = false;
    pdbDacTriggerConfigStruct.enableIntervalTrigger = true;
    PDB_SetDACTriggerConfig(PDB_BASEADDR, PDB_DAC_CHANNEL, &pdbDacTriggerConfigStruct);
    PDB_SetDACTriggerIntervalValue(PDB_BASEADDR, PDB_DAC_CHANNEL, PDB_DAC_INTERVAL_VALUE);

    // Load PDB values.
    PDB_DoLoadValues(PDB_BASEADDR);
}


static void DAC_Configuration(void)
{
    dac_config_t dacConfigStruct;
    dac_buffer_config_t dacBufferConfigStruct;

    DAC_GetDefaultConfig(&dacConfigStruct);
    DAC_Init(DAC_BASEADDR, &dacConfigStruct);
    DAC_Enable(DAC_BASEADDR, true);

    // Configure the DAC buffer.
    DAC_EnableBuffer(DAC_BASEADDR, true);
    DAC_GetDefaultBufferConfig(&dacBufferConfigStruct);
    dacBufferConfigStruct.triggerMode = kDAC_BufferTriggerByHardwareMode;
    DAC_SetBufferConfig(DAC_BASEADDR, &dacBufferConfigStruct);

    // Make sure the read pointer to the start.
    DAC_SetBufferReadPointer(DAC_BASEADDR, 0U);
}


static void Edma_Callback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds)
{
    // Clear Edma interrupt flag.
    EDMA_ClearChannelStatusFlags(DMA_BASEADDR, DMA_CHANNEL, kEDMA_InterruptFlag);

    // Setup transfer
    void * srcAdd = NULL;

    if(pause)
	{
		srcAdd = muteAudioBuffer;
	}

    else
    {
    	sampleIndex += DAC_DATL_COUNT; //TODO: si esto queda aca, tiene que haber una primera transferencia antes.

    	if (sampleIndex == AUDIO_PLAYER_BUFF_SIZE)
		{
    		// If the activeBuffer was completely transferred
			sampleIndex = 0;

			// Load backBuffer as new activeBuffer
			// and clear backBuffer
			void * temp = activeBuffer;
			activeBuffer = backBuffer;
			backBuffer = temp;
			backBufferFree = true;

			// Update the sampleRate with the one loaded in the backBuffer
			AudioPlayer_UpdateSampleRate(backBufferSampleRate);
		}

		srcAdd = (activeBuffer + sampleIndex);
    }

	// EDMA:
    edma_transfer_config_t * config = &audioPlayer_transferConfig;

    // Source
    uint32_t srcWidth = sizeof(uint16_t);

    // Address
    void *destAddr = (void *)DAC_DATA_REG_ADDR;
    uint32_t destWidth =  sizeof(uint16_t);
    uint32_t bytesEachRequest = DAC_DATL_COUNT * sizeof(uint16_t);
    uint32_t transferBytes = DAC_DATL_COUNT * sizeof(uint16_t);
    edma_transfer_type_t type = kEDMA_MemoryToMemory;

    EDMA_PrepareTransfer(config,
						  (void*) srcAdd,
						  srcWidth,
						  destAddr,
						  destWidth,
						  bytesEachRequest,
						  transferBytes,
						  type);

    EDMA_SetTransferConfig(DMA_BASEADDR, DMA_CHANNEL, &audioPlayer_transferConfig, NULL);

    // Enable transfer.
    EDMA_StartTransfer(&audioPlayer_EDMA_Handle);
}


static uint32_t roundedDivide(uint32_t dividend, uint32_t divisor)
{
    // Ensure divisor is not zero to avoid division by zero
    if (divisor == 0)
    {
        return 0; // You can handle this error in your own way
    }

    // Perform the division
    uint32_t quotient = dividend / divisor;
    uint32_t remainder = dividend % divisor;

    // Calculate the rounded result
    if (2 * remainder >= divisor)
    {
        // Round up
        quotient += 1;
    }

    return quotient;
}
