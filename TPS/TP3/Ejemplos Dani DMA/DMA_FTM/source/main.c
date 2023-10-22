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

 	 	 	    hw_DisableInterrupts ();

 	 	 	    PORT_Init();
 	 	 	    FTM_Init ();

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

//  Si CITER =5 -> Primeros 5 valores luego se repite ver SLAST
//	DTY=(1/50e6)*32* sourceBuffer[i];  50MHz x32 prescaler
//	1000-640 us
//	2000-1.28 ms
//  3000-1.92 ms
//	4000-2.56 ms
//	5000-3.20 ms


static uint16_t sourceBuffer[] = { 1000 ,2000,3000,4000,5000,6000,7000,8000};
static uint16_t destinationBuffer[]={0,0,0,0,0,0,0,0,0,0};


	/**************************************************************************/
	/***** Configure the FRDM-K64F LEDs (RED - PTB22, GREEN - PTE26,
	 * BLUE - PTB21) to report events. *****/

	/* Enable the clock for the PORT B and the PORT E. */
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

	/* Configure the MUX option. */

	PORTB->PCR[PIN_BLUE_LED] |= PORT_PCR_MUX(PORT_mGPIO);
	PORTB->PCR[PIN_RED_LED] |= PORT_PCR_MUX(PORT_mGPIO);
	PORTE->PCR[PIN_GREEN_LED] |= PORT_PCR_MUX(PORT_mGPIO);

	/* Select LEDs as outputs. */
	PTB->PDDR |= (1 << PIN_BLUE_LED) | (1 << PIN_RED_LED);
	PTE->PDDR |= (1 << PIN_GREEN_LED);

	/* Turn all the LEDs off. (Active low)*/
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
	//DMAMUX_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(51); // PORTC (Useful for debug sw2 on board)

	/* Enable the eDMA channel 0 and set the FTM CH0 as the DMA request source. */
	DMAMUX->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(20);   // FTM0 CH0

	/* Enable the interrupts for the channel 0. */

	/* Clear all the pending events. */
	NVIC_ClearPendingIRQ(DMA0_IRQn);
	/* Enable the DMA interrupts. */
	NVIC_EnableIRQ(DMA0_IRQn);


	/// ============= INIT TCD0 ===================//
	/* Set memory address for source and destination. */
	DMA0->TCD[0].SADDR = (uint32_t)(sourceBuffer);				   //List of Duties

    //DMA_TCD0_DADDR = (uint32_t)(destinationBuffer);
	DMA0->TCD[0].DADDR = (uint32_t)(&(FTM0->CONTROLS[0].CnV));  // To change FTM Duty


		/* Set an offset for source and destination address. */
	DMA0->TCD[0].SOFF =0x02; // Source address offset of 2 bytes per transaction.
	DMA0->TCD[0].DOFF =0x00; // Destination address offset is 0. (Siempre al mismo lugar)

	/* Set source and destination data transfer size to 16 bits (CnV is 2 bytes wide). */
	DMA0->TCD[0].ATTR = DMA_ATTR_SSIZE(1) | DMA_ATTR_DSIZE(1);

	/*Number of bytes to be transfered in each service request of the channel.*/
	DMA0->TCD[0].NBYTES_MLNO = 0x02;

	/* Current major iteration count (5 iteration of 1 byte each one). */

	//DMA_TCD0_CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(0x05);
	//DMA_TCD0_BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(0x05);

	/* Autosize for Current major iteration count */
	DMA0->TCD[0].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER((sizeof(sourceBuffer)/sizeof(sourceBuffer[0])));
	DMA0->TCD[0].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER((sizeof(sourceBuffer)/sizeof(sourceBuffer[0])));


	//DMA_TCD0_SLAST = 0x00;
    //DMA_TCD0_SLAST = -5*sizeof(uint16_t);
	//DMA_TCD0_SLAST = -((sizeof(sourceBuffer)/sizeof(sourceBuffer[0])*sizeof(uint16_t)));

	/* Autosize SLAST for Wrap Around. This value is added to SADD at the end of Major Loop */
	DMA0->TCD[0].SLAST = -sizeof(sourceBuffer);


    /* DLASTSGA DLAST Scatter and Gatter */
     DMA0->TCD[0].DLAST_SGA = 0x00;

	/* Setup control and status register. */

	DMA0->TCD[0].CSR = DMA_CSR_INTMAJOR_MASK;	//Enable Major Interrupt.

	/* Enable request signal for channel 0. */
	DMA0->ERQ = DMA_ERQ_ERQ0_MASK;

	hw_EnableInterrupts();


	for (;;)
	{

		if(MinorTransferDone)
		{

			MinorTransferDone = 0;

		}
	}

	/* Never leave main */
	return ;
}

/* The blue LED is toggled when a TCD is completed. */
void DMA0_IRQHandler()
{
	/* Clear the interrupt flag. */
	DMA0->CINT |= 0;

	/* Toggle the blue LED. */
	PTB->PTOR |= (1 << PIN_BLUE_LED);

	/* Change the source buffer contents. */
	MinorTransferDone = 1;
}

/* The red LED is toggled when an error occurs. */
void DMA_Error_IRQHandler()
{
	/* Clear the error interrupt flag.*/
	DMA0->CERR |= 0;

	/* Turn the red LED on. */
	PTB->PTOR |= (1 << PIN_RED_LED);
}






