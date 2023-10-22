/*
 * main.c
 *
 *  Created on: May 1, 2015
 *      Author: Juan Pablo VEGA - Laboratorio de Microprocesadores
 */

#include "hardware.h"
#include "PORT.h"
#include "GPIO.h"
#include "FTM.h"
#include "UART.h"
#include "DAC.h"
#include "SysTick.h"
#include "FRDMK64_Board.h"

#define __FOREVER__ 	for(;;)


/* Auxiliary variable used to modify the source buffer on each iteration. */
static uint8_t MinorTransferDone = 0;

//uint16_t sourceBuffer[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
uint16_t sourceBuffer[10] = {0x1234,0x6789,0x1122,0x2233,0x5588,0x2345,0x3145,0x8172,0x6183,0x3756};
uint8_t destinationBuffer[10];






void DMA_Test(void);

int main (void)
{
 	 	 	 	hw_Init();
 	 //	 	 	PORT_Init();
 	 //	 		GPIO_Init();
 	 //	 		DAC_Init();
 	 //	 		SysTick_Init();

 	 	 		DMA_Test();

 	 	// 		hw_DisableInterrupts();

 	 	 		__FOREVER__;

			// Enable interrupts
			//hw_EnableInterrupts();

}


void DMA_Test(void)
{

	hw_DisableInterrupts ();
	/**************************************************************************/
	/***** Configure the FRDM-K64F LEDs (RED - PTB22, GREEN - PTE26,
	 * BLUE - PTB21) to report events. *****/

	/* Enable the clock for the PORT B and the PORT E. */

	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;


	/* Configure the MUX option. */

	//// NO VA PORTA->PCR[4] |= PORT_PCR_ISF_MASK;  //Clear mask

	PORTB->PCR[PIN_BLUE_LED] |= PORT_PCR_MUX(PORT_mGPIO);
	PORTB->PCR[PIN_RED_LED] |= PORT_PCR_MUX(PORT_mGPIO);
	PORTE->PCR[PIN_GREEN_LED] |= PORT_PCR_MUX(PORT_mGPIO);

	/* Select LEDs as outputs. */

	PTB->PDDR |= (1 << PIN_BLUE_LED) | (1 << PIN_RED_LED);
	PTE->PDDR |= (1 << PIN_GREEN_LED);


	/* Turn all the LEDs off. (Active low) */

	PTB->PSOR |= (1 << PIN_BLUE_LED) | (1 << PIN_RED_LED);
	PTE->PSOR |= (1 << PIN_GREEN_LED);



	/**************************************************************************/
	/***** Configure the FRDM-K64F SW2 (PTC6) as the DMA request source. *****/

	/* Enable the clock for the PORT C*/
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;

	/* Configure the SW2 DMA request source on falling edge. */

	PORTC->PCR[6]=0x00;
	PORTC->PCR[6]|=PORT_PCR_MUX(PORT_mGPIO); 		       //Set MUX to GPIO;
	PORTC->PCR[6]|=PORT_PCR_PE(1);          		       //Pull UP/Down  Enable;
	PORTC->PCR[6]|=PORT_PCR_PS(1);          		       //Pull UP
	PORTC->PCR[6]|=PORT_PCR_IRQC(PORT_eDMAFalling);


	/* Enable the clock for the eDMA and the DMAMUX. */
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;

	/* Enable the eDMA channel 0 and set the PORTC as the DMA request source. */
	DMAMUX->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(51);

	/* Enable the interrupts for the channel 0. */
	/* Clear all the pending events. */
	NVIC_ClearPendingIRQ(DMA0_IRQn);
	/* Enable the DMA interrupts. */
	NVIC_EnableIRQ(DMA0_IRQn);


	/// ============= INIT TCD0 ===================//
	/* Set memory address for source and destination. */

	DMA0->TCD[0].SADDR= (uint32_t)(sourceBuffer);
	DMA0->TCD[0].DADDR = (uint32_t)(destinationBuffer);

		/* Set an offset for source and destination address. */
	DMA0->TCD[0].SOFF =0x02; // Source address offset of 2 bytes per transaction.
	DMA0->TCD[0].DOFF =0x01; // Destination address offset of 1 byte per transaction.

	/* Set source and destination data transfer size is 1 byte. */
	DMA0->TCD[0].ATTR = DMA_ATTR_SSIZE(0) | DMA_ATTR_DSIZE(0);

	/*Number of bytes to be transfered in each service request of the channel.*/
	DMA0->TCD[0].NBYTES_MLNO= 0x01;

	/* Current major iteration count (5 iteration of 1 byte each one). */
	DMA0->TCD[0].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(0x05);
	DMA0->TCD[0].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(0x05);



	/* Address for the next TCD to be loaded in the scatter/gather mode. */
	//	tcd[TCD0].SLAST = 0;			// Source address adjustment not used.
	//	tcd[TCD0].DLASTSGA = (uint32_t)&tcd[TCD1];	// The tcd[TCD1] is the next TCD to be loaded.


	DMA0->TCD[0].SLAST = 0x00;
	DMA0->TCD[0].DLAST_SGA = 0x00;

	/* Setup control and status register. */

	DMA0->TCD[0].CSR = DMA_CSR_INTMAJOR_MASK;	//Enable Major Interrupt.

	/* Enable request signal for channel 0. */
	DMA0->ERQ = DMA_ERQ_ERQ0_MASK;

	hw_EnableInterrupts();

	for (;;)
	{

		/* Chk */
		if(MinorTransferDone)
		{
			MinorTransferDone = 0;
		}
	}

	/* Never leave main */
	return ;
}

/* The blue LED is toggled when a TCD is completed. */
__ISR__ DMA0_IRQHandler(void)
{
	/* Clear the interrupt flag. */
	DMA0->CINT |= 0;

	/* Toggle the blue LED. */
	PTB->PTOR |= (1 << PIN_BLUE_LED);

	/* Change the source buffer contents. */
	MinorTransferDone = 1;
}

/* The red LED is toggled when an error occurs. */
__ISR__ DMA_Error_IRQHandler(void)
{
	/* Clear the error interrupt flag.*/
	DMA0->CERR |= 0;

	/* Turn the red LED on. */
	PTB->PTOR |= (1 << PIN_RED_LED);
}











