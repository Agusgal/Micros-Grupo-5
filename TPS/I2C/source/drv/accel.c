
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "i2c.h"
#include "accel.h"
#include "FXOS8700CQ.h"
#include <stdbool.h>
#include "SysTick.h"
#include <math.h>
#include "gpio.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define FXOS8700CQ_UPDATE_TICK_MS       50		//20 Hz
#define FX_PISR_PERIOD_MS       50		//20 Hz
#define FX_WRITE_BUFFER_SIZE    20
#define FX_READ_BUFFER_SIZE     20
#define WHOAMI_VALUE         	0xC7
#define NULL					0
/*******************************************************************************
 * VARIABLE PROTOTYPES WITH LOCAL SCOPE
 ******************************************************************************/


typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
} Coord16_t;

typedef struct {
	float x;
	float y;
	float z;
} Coord_Float_t;

typedef enum {
  FX_Init,
  FX_Running,
  FX_Error
} Acc_Status_t;



// Accelerometer data structure
typedef struct {

  bool                  alreadyInit;            // Driver inicializado
  Acc_Status_t          status;                 // Estado del acelerometro
  uint8_t               counter;				// Counter para FSM

  Coord16_t	          	accelerometer;        // Vector acceleration
  Coord16_t				magnetometer;		 //
  Orient_t				orientation;

  /* I2C message buffers */
  uint8_t               writeBuffer[FX_WRITE_BUFFER_SIZE];
  uint8_t               readBuffer[FX_READ_BUFFER_SIZE];

} Accel_t;

static Accel_t 	Accel;
/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void FX_InitSequence(bool resetCounter);
static void FX_RunningSequence(bool reset);
static void FX_ReadReg(uint8_t reg);
static void FX_WriteReg(uint8_t reg, uint8_t value);
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void FX_ReadReg(uint8_t reg)
{
  Accel.writeBuffer[0] = reg;
  switch(reg)
  {
  	  case MOUT_X_MSB_REG:
  	  case OUT_X_MSB_REG:
  		I2C_InitObject(0, Accel.readBuffer,12,Accel.writeBuffer,1,0x1D);
  		break;

  	  default:
  		I2C_InitObject(0, Accel.readBuffer,1,Accel.writeBuffer,1,0x1D);
  		break;

  }

}

static void FX_WriteReg(uint8_t reg, uint8_t value)
{
  Accel.writeBuffer[0] = reg;
  Accel.writeBuffer[1] = value;
  I2C_InitObject(0,NULL,0,Accel.writeBuffer,2,0x1D);
}

bool FX_I2C_Init()
{
	 if (!Accel.alreadyInit)
	  {
	    // Initialize the I2C driver
	    I2C_InitModule(0);
	    // Initialize and set the periodic service routine
	    SysTick_Init();
	    SysTick_Reg_Callback(FX_PISR,FX_PISR_PERIOD_MS * MS_TO_US);

	    // Raise the already initialized flag
	    Accel.alreadyInit = true;
	  }

	  return Accel.alreadyInit;
}

void FX_InitSequence(bool resetCounter)
{
	if (resetCounter)
		  Accel.counter = 0;

	switch (Accel.counter++)
	{
	case 0: // Read WHO_AM_I register to check device presence
	  FX_ReadReg(WHO_AM_I_REG);
	  break;

	case 1: // Check WHO_AM_I value read matches
	  if (Accel.readBuffer[0] == 0xC7)
	  {
		// Set FXOS to standby mode
		FX_WriteReg(CTRL_REG1, 0);			//FX_WriteReg(CTRL_REG1, 0);
	  }
	  else
	  {
		Accel.status = FX_Error;
	  }
	  break;

	case 2:
		FX_WriteReg(XYZ_DATA_CFG_REG, 0x00);		//+-2g resolution
	  break;

	case 3:
		FX_WriteReg(M_CTRL_REG1, 0x9F);
	  break;

	case 4:
		FX_WriteReg(M_CTRL_REG2, 0x20);
	  break;

	case 5:
		FX_WriteReg(CTRL_REG3, 0x01);
	  break;

	case 6:
		FX_WriteReg(CTRL_REG4, 0x01);
	  break;

	case 7:
		FX_WriteReg(CTRL_REG5, 0x01);
	  break;

	case 8:
		FX_WriteReg(PL_CFG_REG, 0x40);

	case 9: // Set the accelerometer active
		FX_WriteReg(CTRL_REG1, 0x35);
	  break;

	case 10: // Change to running state
		Accel.status = FX_Running;
		break;

	default:
	  break;
	}
}

void FX_RunningSequence(bool resetCounter)
{

  if (resetCounter)
  {
    Accel.counter = 0;
  }

  switch (Accel.counter++)
  {
  	case 0:
  		FX_ReadReg(OUT_X_MSB_REG);
    case 1:


        Accel.accelerometer.x = ((int16_t)(Accel.readBuffer[0] << 8 | Accel.readBuffer[1])) >> 2;  // Compute 14-bit X-axis acceleration output value
        Accel.accelerometer.y = ((int16_t)(Accel.readBuffer[2] << 8 | Accel.readBuffer[3])) >> 2;  // Compute 14-bit X-axis acceleration output value
        Accel.accelerometer.z = ((int16_t)(Accel.readBuffer[4] << 8 | Accel.readBuffer[5])) >> 2;  // Compute 14-bit X-axis acceleration output value

		// Magnetometer data converted to microteslas
    	Accel.magnetometer.x = (int16_t)(Accel.readBuffer[6] << 8 | Accel.readBuffer[7]);  // Compute 16-bit X-axis magnetic output value
    	Accel.magnetometer.y = (int16_t)(Accel.readBuffer[8] << 8 | Accel.readBuffer[9]);  // Compute 16-bit X-axis magnetic output value
    	Accel.magnetometer.z = (int16_t)(Accel.readBuffer[10] << 8 | Accel.readBuffer[11]);  // Compute 16-bit X-axis magnetic output value

    	float north =90+atan2(Accel.magnetometer.y, Accel.magnetometer.x) * 180 / 3.14159;

    	if (north>=180)
    		north-=360;
    	Accel.orientation.norte=north;
    	Accel.orientation.rolido=atan2(Accel.accelerometer.x, Accel.accelerometer.z) * 180 / 3.14159;
    	Accel.orientation.cabeceo=atan2(Accel.accelerometer.y, Accel.accelerometer.z) * 180 / 3.14159;


      break;

    default:
      break;
  }

}
void FX_PISR(void)
{
  if (Accel.status == FX_Init)
    FX_InitSequence(true);
  else if (Accel.status == FX_Running)
    FX_RunningSequence(true);
}

void FX_I2C_Finished(void)
{
  for(uint32_t i = 0 ; i < 1000 ; i++);

  if (Accel.status == FX_Init)
    FX_InitSequence(false);
  else if (Accel.status == FX_Running)
	  FX_RunningSequence(false);
}

Orient_t getAccelData()
{
	return Accel.orientation;
}
