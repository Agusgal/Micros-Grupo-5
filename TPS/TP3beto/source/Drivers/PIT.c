/*
 * PIT.c
 *
 *  Created on: 5 nov. 2023
 *      Author: agus
 */

#include "PIT.h"
#include "hardware.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
static PIT_Type* PIT_ptr = PIT;
static SIM_Type* sim_ptr = SIM;

void (*callback0)(void);
void (*callback1)(void);
void (*callback2)(void);


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void PIT_init(void)
{
	// Clock Gating
	sim_ptr->SCGC6 |= SIM_SCGC6_PIT_MASK;

	//Config
	PIT_ptr->MCR &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK);
	PIT_ptr->MCR |= (PIT_MCR_MDIS(0) | PIT_MCR_FRZ(1));
}


void PIT_set_Timer(uint8_t id, uint32_t value, void(*clb)(void))
{
	//podria ser mejorado...
	switch (id)
	{
		case 0:
			NVIC_EnableIRQ(PIT0_IRQn);
			callback0 = clb;
			break;
		case 1:
			NVIC_EnableIRQ(PIT1_IRQn);
			callback1 = clb;
			break;
		case 2:
			NVIC_EnableIRQ(PIT2_IRQn);
			callback2 = clb;
			break;

	}

	PIT_ptr->CHANNEL[id].LDVAL = (PIT_LDVAL_TSV(value - 1));

	PIT_ptr->CHANNEL[id].TCTRL &= ~(PIT_TCTRL_CHN_MASK);
	PIT_ptr->CHANNEL[id].TCTRL |=(PIT_TCTRL_CHN(0));

	PIT_ptr->CHANNEL[id].TCTRL &= ~(PIT_TCTRL_TIE_MASK);
	PIT_ptr->CHANNEL[id].TCTRL |=(PIT_TCTRL_TIE(0));

	PIT_ptr->CHANNEL[id].TCTRL &= ~(PIT_TCTRL_TEN_MASK);
	PIT_ptr->CHANNEL[id].TCTRL |=(PIT_TCTRL_TEN(0));
}


void PIT_Start_Timer(uint8_t id)
{
	PIT_ptr->CHANNEL[id].TCTRL &= ~(PIT_TCTRL_TIE_MASK);
	PIT_ptr->CHANNEL[id].TCTRL |=(PIT_TCTRL_TIE(1));

	PIT_ptr->CHANNEL[id].TCTRL &= ~(PIT_TCTRL_TEN_MASK);
	PIT_ptr->CHANNEL[id].TCTRL |=(PIT_TCTRL_TEN(1));
}

void PIT_Stop_Timer(uint8_t id)
{
	PIT_ptr->CHANNEL[id].TCTRL &= ~(PIT_TCTRL_TIE_MASK);
	PIT_ptr->CHANNEL[id].TCTRL |=(PIT_TCTRL_TIE(0));

	PIT_ptr->CHANNEL[id].TCTRL &= ~(PIT_TCTRL_TEN_MASK);
	PIT_ptr->CHANNEL[id].TCTRL |=(PIT_TCTRL_TEN(0));
}

/*******************************************************************************
 *******************************************************************************
                       	   	   IRQs
 *******************************************************************************
 ******************************************************************************/

void PIT0_IRQHandler(void)
{
	callback0();

	PIT_ptr->CHANNEL[0].TFLG = PIT_TFLG_TIF(1);
}

void PIT1_IRQHandler(void)
{
	callback1();

	PIT_ptr->CHANNEL[1].TFLG = PIT_TFLG_TIF(1);
}

void PIT2_IRQHandler(void)
{
	callback2();

	PIT_ptr->CHANNEL[2].TFLG = PIT_TFLG_TIF(1);
}

/*******************************************************************************
 ******************************************************************************/
