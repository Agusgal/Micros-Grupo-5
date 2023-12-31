/*
 * CMP.c
 *
 *  Created on: nov. 6 2023
 *      Author: agus
 */

#include "Drivers/CMP.h"
#include "hardware.h"
#include "Drivers/board.h"
#include "Drivers/gpio.h"


#define CMP_ANALOG_IN      1U           // PTC7, CMP0_IN1				//PTC3, CMP1_IN1
#define CMP_ANALOG_REF     7U			// elijo dac
#define HYST_MASK          0U
#define DAC_VALUE         31U
#define VREF_SOURCE_VINX   0U

							//CMP0_IN1			//CMP1_IN1
uint8_t CMP_in_pins[] = {PORTNUM2PIN(PC, 7), PORTNUM2PIN(PC, 3)};

							//CMP0_OUT			//CMP1_OUT
uint8_t CMP_out_pins[] = {PORTNUM2PIN(PC, 5), PORTNUM2PIN(PC, 4)};


CMP_Type * arrayP2CMP[] = CMP_BASE_PTRS;
IRQn_Type arrayCMPirqs[] = CMP_IRQS;
CMP_Type* base;


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/



static void enableInterrupts();
static void configurePins(uint8_t id);
void initCMP(cmp_ids id);
static void configureInputs(uint8_t positiveInput, uint8_t negativeInput);
static void configureDAC();


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


void CMP_init(cmp_ids id)
{

	base = arrayP2CMP[id];
	SIM->SCGC4 |= SIM_SCGC4_CMP_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	configurePins(id);		// CMP0

	// CMP0_OUT Enabled
	base->CR1 |= CMP_CR1_OPE_MASK;

	// Hysteresis Enabled
	base->CR0 |= CMP_CR0_HYSTCTR(HYST_MASK) | CMP_CR0_FILTER_CNT(4);

	base->FPR = 0;
	base->FPR |= CMP_FPR_FILT_PER(100);

	configureDAC();
	configureInputs(CMP_ANALOG_IN, CMP_ANALOG_REF);

	base->CR1 |= CMP_CR1_EN_MASK;		// Enable Module
	enableInterrupts();
	NVIC_EnableIRQ(arrayCMPirqs[id]);
}


void configurePins(uint8_t id)
{
	uint8_t cmp_in_mux = 0;
	uint8_t cmp_out_mux = 6;

	PORT_Type* 	ports[] = PORT_BASE_PTRS;
	pin_t 		pin = CMP_in_pins[id];
	uint8_t 	alt = cmp_in_mux;
	ports[PIN2PORT(pin)]->PCR[PIN2NUM(pin)] |= PORT_PCR_MUX(alt);

	pin = CMP_out_pins[id];
	alt = cmp_out_mux;
	ports[PIN2PORT(pin)]->PCR[PIN2NUM(pin)] |= PORT_PCR_MUX(alt);

}


void enableInterrupts()
{
	base->SCR |= (CMP_SCR_IEF_MASK | CMP_SCR_IER_MASK);
}

void configureInputs(uint8_t positiveInput, uint8_t negativeInput)
{
	uint8_t mask = base->MUXCR;

	// Delete Selectors
	mask &= ~(uint8_t)(CMP_MUXCR_PSEL_MASK | CMP_MUXCR_MSEL_MASK);
	mask |= (CMP_MUXCR_MSEL(negativeInput) | CMP_MUXCR_PSEL(positiveInput));
	base->MUXCR = mask;
}


void configureDAC()
{
	base->DACCR |= (CMP_DACCR_VOSEL(DAC_VALUE) | CMP_DACCR_DACEN_MASK | CMP_DACCR_VRSEL_MASK);
}


void CMP0_IRQHandler(void)
{
	CMP_Type* base = arrayP2CMP[CMP_0];
	base->SCR |= (CMP_SCR_CFF_MASK | CMP_SCR_CFR_MASK);
}


/*******************************************************************************
 ******************************************************************************/
