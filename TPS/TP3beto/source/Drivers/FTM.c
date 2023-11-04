/*******************************************************************************
  @file     ftm.c
  @brief    FTM Driver
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <MK64F12.h>
#include "FTM.h"
#include "gpio.h"
#include "hardware.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define CHANNEL_MASK(x)		(0x00000001 << (x))
#define SYSTEM_CLOCK		(1)
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

static FTM_Callback_t		FTM_OVF_Callbacks[FTM_COUNT];
static CH_Callback_t		CH_Callbacks[FTM_COUNT][FTM_CH_COUNT];
/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

// FTM modules interruption service routines
static void FTM_IRQ_Dispatcher(FTM_Module_t module);

static void FTM_CH_Mux(FTM_Module_t module, FTM_Channel_t channel);

static void FTM_CH_DSE(FTM_Module_t module, FTM_Channel_t channel);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/


// Pointers to FTM modules
static FTM_Type *	FTM_Modules[] = FTM_BASE_PTRS;

// FTMIRQn for NVIC Enabling
static const uint8_t FTM_IRQs[] = FTM_IRQS;

// FTM Overflow Callbacks

static const pin_t	FTM_CH_PINS [FTM_COUNT][FTM_CH_COUNT] = {
	//	Channel 0			Channel 1			Channel 2			Channel 3		Channel 4			Channel 5			Channel 6			Channel 7
	{ PORTNUM2PIN(PC,1),  PORTNUM2PIN(PA,4),  PORTNUM2PIN(PC,3), PORTNUM2PIN(PC,4), PORTNUM2PIN(PD,4), PORTNUM2PIN(PD,5), PORTNUM2PIN(PD,6)	, PORTNUM2PIN(PD,7)  },	// FTM0
	{ PORTNUM2PIN(PA,12), PORTNUM2PIN(PA,13), 0				   , 0				  , 0				 , 0			   	, 0				   	, 0				     }, // FTM1
	{ PORTNUM2PIN(PB,18), PORTNUM2PIN(PB,19), 0				   , 0				  , 0				 , 0			   	, 0				   	, 0				     }, // FTM2
	{ PORTNUM2PIN(PD,0),  PORTNUM2PIN(PD,1),  PORTNUM2PIN(PD,2), PORTNUM2PIN(PD,3), PORTNUM2PIN(PC,8), PORTNUM2PIN(PC,9), PORTNUM2PIN(PC,10), PORTNUM2PIN(PC,11) }  // FTM3
};

// FTM Channel Pin MUX Alternatives
static const uint8_t	FTM_CH_MUX_ALTS[FTM_COUNT][FTM_CH_COUNT] = {
	// Ch0 Ch1 Ch2 Ch3 Ch4 Ch5 Ch6 Ch7
	{  4,  3,  4,  4,  4,  4,  4,  4  }, // FTM0
	{  3,  3,  0,  0,  0,  0,  0,  0  }, // FTM1
	{  3,  3,  0,  0,  0,  0,  0,  0  }, // FTM2
	{  4,  4,  4,  4,  3,  3,  3,  3  }  // FTM3
};

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/*******************************************************************************
 *                        FTM MODULE FUNCTIONS
 ******************************************************************************/

void FTM0_Init(FTM_PS_t prescaler, FTM_Callback_t callback)
{
	FTM_Init(FTM_0,prescaler,0xFFFF,callback);
}

void FTM_Init(FTM_Module_t module, FTM_PS_t prescaler, uint16_t modulo, FTM_Callback_t callback)
{
	// Clock gating enable
	switch(module)
	{
		case FTM_0:
			SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;
			break;
			
		case FTM_1:
			SIM->SCGC6 |= SIM_SCGC6_FTM1_MASK;
			break;

		case FTM_2:
			SIM->SCGC3 |= SIM_SCGC3_FTM2_MASK;
			SIM->SCGC6 |= SIM_SCGC6_FTM2_MASK;
			break;

		case FTM_3:
			SIM->SCGC3 |= SIM_SCGC3_FTM3_MASK;
			break;

		default:
			break;
	}

	// Port Clock Gating
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

	// Enable Interrupts on NVIC
	NVIC_EnableIRQ(FTM_IRQs[FTM_0]);
	NVIC_EnableIRQ(FTM_IRQs[FTM_1]);
	NVIC_EnableIRQ(FTM_IRQs[FTM_2]);
	NVIC_EnableIRQ(FTM_IRQs[FTM_3]);

	// Enable CNTIN and MOD configuration
	FTM_Modules[module]->PWMLOAD = FTM_PWMLOAD_LDOK(1) | CHANNEL_MASK(module);


	// Free-running counter and prescaler
	FTM_Modules[module]->SC = FTM_SC_PS(prescaler);
	FTM_Modules[module]->CNTIN = 0;
	FTM_Modules[module]->CNT=0;
	FTM_Modules[module]->MOD = modulo-1;


	// Enable advanced mode
	//FTM_Modules[module]->MODE |= FTM_MODE_FTMEN(1);

	// Add overflow callback
	if (callback)
	{
		// Enables the timer overflow interrupt for the FTM instance
		FTM_Modules[module]->SC |= FTM_SC_TOIE(1);
		// Registers the callback to be called when timer overflow
		FTM_OVF_Callbacks[module] = callback;
	}
}


void FTM_Start(FTM_Module_t module)
{
	FTM_Modules[module]->SC |= FTM_SC_CLKS(1);
}

void FTM_Stop(FTM_Module_t module)
{
	FTM_Modules[module]->SC &= (~FTM_SC_CLKS_MASK);
}

void FTM_Restart(FTM_Module_t module)
{
	FTM_Stop(module);
	FTM_Modules[module]->CNT = 0;
	FTM_Start(module);
}


uint16_t FTM_GetCount(FTM_Module_t module)
{
	return FTM_Modules[module]->CNT;
}

/*******************************************************************************
 *                        FTM CHANNEL FUNCTIONS
 ******************************************************************************/

uint16_t FTM_CH_GetCount(FTM_Module_t module, FTM_Channel_t channel)
{
	return FTM_Modules[module]->CONTROLS[channel].CnV;
}

void FTM_CH_SetCount(FTM_Module_t module, FTM_Channel_t channel,uint16_t count)
{
	FTM_Modules[module]->CONTROLS[channel].CnV=FTM_CnV_VAL(count);
}

void FTM_CH_AddCallback(FTM_Module_t module, FTM_Channel_t channel, CH_Callback_t callback)
{
	FTM_Modules[module]->CONTROLS[channel].CnSC |= FTM_CnSC_CHIE(1);

	if (callback)
	{
		CH_Callbacks[module][channel] = callback;
	}
}

void FTM_CH_EnableDMA(FTM_Module_t module, FTM_Channel_t channel)
{
	FTM_Modules[module]->CONTROLS[channel].CnSC = (	FTM_Modules[module]->CONTROLS[channel].CnSC & ~(FTM_CnSC_DMA_MASK)) | (FTM_CnSC_DMA(1));
	FTM_Modules[module]->CONTROLS[channel].CnSC = (	FTM_Modules[module]->CONTROLS[channel].CnSC & ~FTM_CnSC_CHIE_MASK) | FTM_CnSC_CHIE(1);		// desp vemos si queda comentada
}

uint32_t * FTM_CH_GetCnVPointer(FTM_Module_t module, FTM_Channel_t channel)
{
	return &(FTM_Modules[module]->CONTROLS[channel].CnV);
}

/********************* INPUT CAPTURE *********************/

void FTM_CH_IC_Init(FTM_Module_t module, FTM_Channel_t channel, FTM_IC_Mode_t mode,CH_Callback_t callback)
{
	// Channel set to input capture on given edge/s
	FTM_Modules[module]->CONTROLS[channel].CnSC = FTM_CnSC_ELSA(mode == FTM_IC_RISING_EDGE ? 0 : 1) | FTM_CnSC_ELSB(mode == FTM_IC_FALLING_EDGE ? 0 : 1);
	
	// Pin MUX alternative
	FTM_CH_Mux(module, channel);


	if (callback)
	{
		FTM_Modules[module]->CONTROLS[channel].CnSC |= FTM_CnSC_CHIE(1);
		CH_Callbacks[module][channel] = callback;
	}
}

/********************* OUTPUT COMPARE  *********************/

void FTM_CH_OC_Init(FTM_Module_t module, FTM_Channel_t channel, FTM_OC_Mode_t mode, bool outInit,CH_Callback_t callback)
{

	FTM_Modules[module]->CONTROLS[channel].CnSC = FTM_CnSC_MSB(0) | FTM_CnSC_MSA(1) | FTM_CnSC_ELSB(mode == FTM_OC_TOGGLE ? 0 : 1) | FTM_CnSC_ELSA(mode == FTM_OC_CLEAR ? 0 : 1);

	// Sets the initial value of the output channel
	uint32_t outputMask = CHANNEL_MASK(channel);
	FTM_Modules[module]->OUTINIT = outInit ? (FTM_Modules[module]->OUTINIT | outputMask) : (FTM_Modules[module]->OUTINIT & (~outputMask));

	// Pin MUX alternative
	FTM_CH_Mux(module, channel);

	if (callback)
	{
		FTM_Modules[module]->CONTROLS[channel].CnSC |= FTM_CnSC_CHIE(1);
		CH_Callbacks[module][channel] = callback;
	}
}

void FTM_CH_OC_Start(FTM_Module_t module, FTM_Channel_t channel, uint16_t count)
{
	// Forces the output channel to its initial value registered during the initialization process
	FTM_Modules[module]->MODE |= FTM_MODE_INIT(1);

	FTM_Modules[module]->CONTROLS[channel].CnV = FTM_Modules[module]->CNT + count;
	FTM_Modules[module]->PWMLOAD |= CHANNEL_MASK(channel);
	FTM_Modules[module]->OUTMASK &= (~CHANNEL_MASK(channel));
}

void FTM_CH_OC_Stop(FTM_Module_t module, FTM_Channel_t channel)
{
	FTM_Modules[module]->PWMLOAD &= (~CHANNEL_MASK(channel));
	FTM_Modules[module]->OUTMASK |= CHANNEL_MASK(channel);
}

/********************* PULSE WIDTH MODULATION  *********************/

void FTM_CH_PWM_Init(FTM_Module_t module, FTM_Channel_t channel, FTM_PWM_Mode_t mode, FTM_PWM_Align_t alignment, uint16_t duty, uint16_t period,CH_Callback_t callback)
{

	// Configure up or up/down counter
	FTM_Modules[module]->SC |= FTM_SC_CPWMS(alignment == FTM_PWM_CENTER_ALIGNED ? 1 : 0);
	FTM_Modules[module]->CONTROLS[channel].CnSC = FTM_CnSC_MSB(1) | FTM_CnSC_ELSB(1) | FTM_CnSC_ELSA(mode == FTM_PWM_LOW_PULSES ? 1 : 0);
	
	// Enable changes on MOD, CNTIN and CnV
	FTM_Modules[module]->PWMLOAD = FTM_PWMLOAD_LDOK(1) | 0x0F;// | CHANNEL_MASK(channel);

	// Configure PWM period and duty
	FTM_Modules[module]->CNTIN = 0x00;
	FTM_Modules[module]->CNT = 0x00;
	FTM_Modules[module]->MOD = period-1;
	FTM_Modules[module]->CONTROLS[channel].CnV = FTM_CnV_VAL(duty);

	// Pin MUX alternative
	FTM_CH_Mux(module, channel);

	//Set Drive Strength Enable
	FTM_CH_DSE(module, channel);

	// Enable Synchronization
	//FTM_Modules[module]->COMBINE |= (FTM_COMBINE_SYNCEN0_MASK << (8 * (channel / 2)));
	//FTM_Modules[module]->SYNCONF |= FTM_SYNCONF_SYNCMODE_MASK | FTM_SYNCONF_SWWRBUF_MASK;
	// Sync when CNT == MOD - 1
	//FTM_Modules[module]->SYNC |= FTM_SYNC_CNTMAX_MASK;

	if (callback)
	{
		FTM_Modules[module]->CONTROLS[channel].CnSC |= FTM_CnSC_CHIE(1);
		CH_Callbacks[module][channel] = callback;
	}

}

void FTM_PWM_SetDuty(FTM_Module_t module, FTM_Channel_t channel, uint16_t duty)
{
	//FTM_Modules[module]->SYNC |= FTM_SYNC_SWSYNC_MASK;
	FTM_Modules[module]->CONTROLS[channel].CnV = duty;
}

void FTM_PWM_ON(FTM_Module_t module, FTM_Channel_t channel)
{
	//FTM_Modules[module]->SYNC |= FTM_SYNC_SWSYNC_MASK;
	FTM_Modules[module]->OUTMASK &= (~CHANNEL_MASK(channel));
}

void FTM_PWM_OFF(FTM_Module_t module, FTM_Channel_t channel)
{
	//FTM_Modules[module]->SYNC |= FTM_SYNC_SWSYNC_MASK;
	FTM_Modules[module]->OUTMASK |= CHANNEL_MASK(channel);
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void FTM_IRQ_Dispatcher(FTM_Module_t module)
{
	if (FTM_Modules[module]->SC & FTM_SC_TOF_MASK)		// Overflow del Free Running Counter
	{
		if (FTM_Modules[module]->SC & FTM_SC_TOIE_MASK)
		{
			// Clear the interruption flag
			FTM_Modules[module]->SC &= (~FTM_SC_TOF_MASK);

			// Calls the callback registered (if any)
			FTM_Callback_t callback = FTM_OVF_Callbacks[module];
			if (callback)
			{
				callback();
			}
		}
	}
	else	//Cumple condicion de OC,IC,PWM
	{
		uint32_t status = FTM_Modules[module]->STATUS;

		// Clear flags
		FTM_Modules[module]->STATUS = 0;

		for (FTM_Channel_t channel = 0; channel < FTM_CH_COUNT; channel++)
		{
			// LLamo al callback si corresponde
			CH_Callback_t callback = CH_Callbacks[module][channel];

			if (callback && (status & CHANNEL_MASK(channel)))
				callback(FTM_Modules[module]->CONTROLS[channel].CnV);
		}

	}

}

 __ISR__ FTM0_IRQHandler(void)
{
	FTM_IRQ_Dispatcher(FTM_0);
}

__ISR__ FTM1_IRQHandler(void)
{
	FTM_IRQ_Dispatcher(FTM_1);
}

__ISR__ FTM2_IRQHandler(void)
{
	FTM_IRQ_Dispatcher(FTM_2);
}

__ISR__ FTM3_IRQHandler(void)
{
	FTM_IRQ_Dispatcher(FTM_3);
}

static void FTM_CH_Mux(FTM_Module_t module, FTM_Channel_t channel)
{
	PORT_Type* 	ports[] = PORT_BASE_PTRS;
	pin_t 		pin = FTM_CH_PINS[module][channel];
	uint8_t 	alt = FTM_CH_MUX_ALTS[module][channel];
	ports[PIN2PORT(pin)]->PCR[PIN2NUM(pin)] |= PORT_PCR_MUX(alt);
}

static void FTM_CH_DSE(FTM_Module_t module, FTM_Channel_t channel)
{
	PORT_Type* 	ports[] = PORT_BASE_PTRS;
	pin_t 		pin = FTM_CH_PINS[module][channel];
	ports[PIN2PORT(pin)]->PCR[PIN2NUM(pin)] |= PORT_PCR_DSE(1);
}
