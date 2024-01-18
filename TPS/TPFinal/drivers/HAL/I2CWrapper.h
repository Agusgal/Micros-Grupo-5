/*
 * I2CWrapper.h
 *
 *  Created on: 8 ene. 2024
 *      Author: Agus
 */

#ifndef DRIVERS_HAL_I2CWRAPPER_H_
#define DRIVERS_HAL_I2CWRAPPER_H_

#include <stdio.h>
#include "board.h"

#include "clock_config.h"
#include "fsl_i2c.h"
#include "fsl_port.h"

//#include "OLEDdisplay.h"


#define ACCEL_I2C_CLK_SRC     I2C0_CLK_SRC
#define ACCEL_I2C_CLK_FREQ    CLOCK_GetFreq(I2C0_CLK_SRC)
#define I2C_BAUDRATE          400000

void I2C_InitModule(void);
void BOARD_InitPins(void);
void BOARD_I2C_ConfigurePins(void);


#endif /* DRIVERS_HAL_I2CWRAPPER_H_ */
