/***************************************************************************//**
  @file     main.c
  @brief    FW main
  @author   Nicolás Magliola
 ******************************************************************************/

#include "hardware.h"
#include "../source/drv/CAN_SPI.h"
#include "../source/drv/gpio.h"

void App_Init (void);
void App_Run (void);


int main (void)
{
    hw_Init();

    //gpioMode(I2C_TP_PIN,OUTPUT);
	//gpioMode(CAN_TP_PIN,OUTPUT);
	//gpioWrite(I2C_TP_PIN,HIGH);
	//gpioWrite(CAN_TP_PIN,HIGH);
	//gpioToggle(I2C_TP_PIN);

    hw_DisableInterrupts();
    App_Init(); /* Program-specific setup */
    hw_EnableInterrupts();
    CAN_SPI_Init ();

    __FOREVER__
        App_Run(); /* Program-specific loop  */
}
