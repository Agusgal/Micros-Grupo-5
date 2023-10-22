
#include "SysTick.h"
#include "ADC.h"
#include <stdio.h>
#include "UART.h"

void DAC_PISR (void);
__ISR__ SysTick_Handler (void)
{
	DAC_PISR();
}
void DAC_PISR (void)
{

	static uint16_t k = 0;

	DAC_SetData(DAC0, k++);

	k %= 0x0FFF;


}


void SysTick_Init (void)
{
	SysTick->CTRL = 0x00;
	SysTick->LOAD = 100-1;
	SysTick->VAL  = 0x00;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}


