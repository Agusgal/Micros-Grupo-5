
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "i2c.h"
#include "accel.h"
#include "FXOS8700CQ.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define CALIBRATION_COUNT 100

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH LOCAL SCOPE
 ******************************************************************************/
typedef struct {
	uint8_t x;
	uint8_t y;
	uint8_t z;
} Coord8_t;

typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t z;
} Coord16_t;

typedef struct {
	uint16_t x14;		// 14 bit data
	uint16_t y14;
	uint16_t z14;
	uint8_t x8;		// 8 bit data
	uint8_t y8;
	uint8_t z8;
	Coord8_t offset;
} Acc_Out_t;

typedef struct {
	uint16_t x14;		// 14 bit data
	uint16_t y14;
	uint16_t z14;
	uint8_t x8;		// 8 bit data
	uint8_t y8;
	uint8_t z8;
	Coord8_t offset;
} Mag_Out_t;

typedef uint8_t Reg_t;

typedef struct {
	Reg_t reg;
	I2C_Mode_t mode;
} Step_t;

Acc_Out_t Acc_Data;
Mag_Out_t Mag_Data;
uint8_t Mag_Calib=0;
/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
uint8_t FXOS8700CQ_Calibration();
uint8_t FX_Mag_Calibration();

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

void I2C_Read_FX_Reg(uint8_t reg, uint8_t * read_buffer)
{
	switch (reg)
	{
		case OUT_X_MSB_REG:
		case MOUT_X_MSB_REG:
			I2C_InitObject(0,I2C_Write,read_buffer,1,NULL,0,0x1D,reg);
			break;
		default:
			I2C_InitObject(0,I2C_Write,read_buffer,1,NULL,0,0x1D,reg);
			break;

	}
}

void I2C_Write_FX_Reg(uint8_t reg,uint8_t * write_buffer)
{
	I2C_InitObject(0,I2C_Write,NULL,0,write_buffer,
						1,0x1D,reg);
}

void FXOS8700CQ_Init()
{
	uint8_t data[1];
	I2C_Read_FX_Reg(WHO_AM_I_REG,data);
	if (data[0] == 0xC7)
	{
		data[0]=0x0;
		I2C_Write_FX_Reg(CTRL_REG1, data);  // write 0x00 to accelerometer control register 1 to place FXOS8700CQ into standby mode
		data[0]=0x0;
		I2C_Write_FX_Reg(XYZ_DATA_CFG_REG, data);  // +/-2g range with 0.244mg/LSB
		data[0]=0x1F;
		I2C_Write_FX_Reg(M_CTRL_REG1,data);  // Hybrid mode (accelerometer + magnetometer), max OSR
		data[0]=0x02;
		I2C_Write_FX_Reg(CTRL_REG2, data);  // High Resolution mode
		data[0]=0x01;
		I2C_Write_FX_Reg(CTRL_REG3, data);  // Open-drain, active low interrupt
		data[0]=0x01;
		I2C_Write_FX_Reg(CTRL_REG4, data);  // Enable DRDY interrupt
		data[0]=0x01;
		I2C_Write_FX_Reg(CTRL_REG5, data);  // DRDY interrupt routed to INT1 - PTC6
		data[0]=0x35;
		I2C_Write_FX_Reg(CTRL_REG1, data);  // ODR = 3.125Hz, Reduced noise, Active mode
		FXOS8700CQ_Calibration();
	}
}

uint8_t FXOS8700CQ_Calibration()
{
	I2C_Write_FX_Reg(CTRL_REG1, 0x00);  // Standby mode
	uint8_t Data [6] = {0};
	I2C_Read_FX_Reg(OUT_X_MSB_REG,Data);  // Read data output registers 0x01-0x06

	Acc_Data.x8 = Data[0];
	Acc_Data.y8 = Data[2];
	Acc_Data.z8 = Data[4];
	Acc_Data.x14 = ((int16_t)(Data[0] << 8 | Data[1])) >> 2;  // Compute 14-bit X-axis acceleration output value
	Acc_Data.y14 = ((int16_t)(Data[2] << 8 | Data[3])) >> 2;  // Compute 14-bit Y-axis acceleration output value
	Acc_Data.z14 = ((int16_t)(Data[4] << 8 | Data[5])) >> 2;  // Compute 14-bit Z-axis acceleration output value

	Acc_Data.offset.x = Acc_Data.x14 / 8 * (-1);  // Compute X-axis offset correction value
	Acc_Data.offset.y = Acc_Data.y14 / 8 * (-1);  // Compute Y-axis offset correction value
	Acc_Data.offset.z = (Acc_Data.z14 - SENSITIVITY_2G) / 8 * (-1);  // Compute Z-axis offset correction value

	I2C_Write_FX_Reg(OFF_X_REG,&(Acc_Data.offset.x));
	I2C_Write_FX_Reg(OFF_Y_REG,&(Acc_Data.offset.y));
	I2C_Write_FX_Reg(OFF_Z_REG,&(Acc_Data.offset.z));

	Data[0]= 0x35;
	I2C_Write_FX_Reg(CTRL_REG1, Data);  // Active mode again
	Mag_Calib=1;
}

uint8_t FX_Mag_Calibration()
{
	uint8_t Data [6];
	I2C_Read_FX_Reg(MOUT_X_MSB_REG, Data);  // Read data output registers 0x33 - 0x38
    Mag_Data.x14 = (int16_t)(Data[0] << 8 | Data[1]);  // Compute 16-bit X-axis magnetic output value
    Mag_Data.y14 = (int16_t)(Data[2] << 8 | Data[3]);  // Compute 16-bit Y-axis magnetic output value
    Mag_Data.z14 = (int16_t)(Data[4] << 8 | Data[5]);  // Compute 16-bit Z-axis magnetic output value

    static Coord16_t min;
    static Coord16_t max;
    static int i=0;

    if (i<=CALIBRATION_COUNT)
    {
		// Assign first sample to maximum and minimum values
		 if (i == 0)
		 {
			 max.x = Mag_Data.x14;
			 max.y = Mag_Data.y14;
			 max.z = Mag_Data.z14;
			 min.x = Mag_Data.x14;
			 min.y = Mag_Data.y14;
			 min.z = Mag_Data.z14;
		 }

		 // Check to see if current sample is the maximum or minimum X-axis value
		 if (Mag_Data.x14 > max.x)
			 max.x= Mag_Data.x14;
		 if (Mag_Data.x14 < min.x)
			 min.x = Mag_Data.x14;

		 // Check to see if current sample is the maximum or minimum Y-axis value
		 if (Mag_Data.y14 > max.y)
			 max.y = Mag_Data.y14;
		 if (Mag_Data.y14 < min.y)
			 min.y = Mag_Data.y14;

		 // Check to see if current sample is the maximum or minimum Z-axis value
		 if (Mag_Data.z14 > max.z)
			 max.z = Mag_Data.z14;
		 if (Mag_Data.z14 < min.z)
			 min.z = Mag_Data.z14;

		 i++;
	}
    else
    {
    	Coord16_t avg;
		avg.x = (max.x + min.x) / 2;
		avg.y = (max.y + min.y) / 2;
		avg.z = (max.z + min.z) / 2;

		// Left-shift by one as magnetometer offset registers are 15-bit only, left justified
		avg.x <<= 1;
		avg.y <<= 1;
		avg.z <<= 1;
		uint8_t Data[]={0x0};
		I2C_Write_FX_Reg(CTRL_REG1, Data);  // Standby mode to allow writing to the offset registers

		Data[0]=(uint8_t)(avg.x & 0xFF);
		I2C_Write_FX_Reg(MOFF_X_LSB_REG,Data);

		Data[0]=(uint8_t)((avg.x >> 8) & 0xFF);
		I2C_Write_FX_Reg(MOFF_X_MSB_REG, Data);

		Data[0]=(uint8_t)(avg.y & 0xFF);
		I2C_Write_FX_Reg(MOFF_Y_LSB_REG, Data);

		Data[0]=(uint8_t)((avg.y >> 8) & 0xFF);
		I2C_Write_FX_Reg(MOFF_Y_MSB_REG, Data);

		Data[0]=(uint8_t)(avg.z & 0xFF);
		I2C_Write_FX_Reg(MOFF_Z_LSB_REG, Data);

		Data[0]=(uint8_t)((avg.z >> 8) & 0xFF);
		I2C_Write_FX_Reg(MOFF_Z_MSB_REG, Data);

		Data[0]=0x35;
		I2C_Write_FX_Reg(CTRL_REG1, Data);

    }

}
