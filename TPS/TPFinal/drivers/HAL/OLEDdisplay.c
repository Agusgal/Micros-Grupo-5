/***************************************************************************//**
  @file     OLEDdisplay.c
  @brief    OLED display functions
  @author   Grupo 5
 ******************************************************************************/

/*----esta modificada para usar el I2C0, en caso de utilizar otro cambiar todo lo que diga I2C1 por la base del I2C usado--- */
/*---- BASADO EN fsl_SSD1306.h para SPI de NXP ------*/

#include "OLEDdisplay.h"
#include "I2CWrapper.h"
#include "fsl_i2c.h"

#include "timer.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief OLED buffer */
static uint8_t OLED_Buffer[(OLED_WIDTH * OLED_HEIGHT) / 8];
static uint8_t OLED_Scroll_Buffer[2][OLED_WIDTH * 8];

static bool isInit = false;
int OLEDtimerClbID = -1;

static bool roll = false;
static char* screenString;

static void rollCLB(void);

static void shiftPageLeft(uint8_t page, uint8_t Scale);
static int OLED_Render_Scroll_Char (uint8_t X_axis, uint8_t Y_axis, uint8_t SC, int8_t String, uint8_t Scale);
static void toggleRoll(void);


/*******************************************************************************
 * Code
 ******************************************************************************/

static void OLED_Command (uint8_t Cmd)
{
	i2c_master_transfer_t xfer = {0};

	xfer.data = &Cmd;
	xfer.dataSize = sizeof(Cmd);
	xfer.flags = kI2C_TransferDefaultFlag;
	xfer.slaveAddress = SSD1306_ADDRESS_1;
	xfer.direction = kI2C_Write;
	xfer.subaddress = 0x0;
	xfer.subaddressSize = 1;

	I2C_MasterTransferBlocking(I2C0, &xfer);

}

static void OLED_Data (uint8_t *Data)
{
	i2c_master_transfer_t xfer = {0};

	/*Start Transfer*/
	xfer.data = Data;
	xfer.dataSize = sizeof(OLED_Buffer);
	xfer.flags = kI2C_TransferDefaultFlag;
	xfer.slaveAddress = SSD1306_ADDRESS_1;
	xfer.direction = kI2C_Write;
	xfer.subaddress = 0x40;
	xfer.subaddressSize = 1;

	I2C_MasterTransferBlocking(I2C0, &xfer);

}

static void OLED_Reset (void)
{
	OLED_Command(OLED_DISPLAYON);
//	for(int i=0; i<1000; i++);					//delay 1ms
	OLED_Command(OLED_DISPLAYOFF);
//	for(int i=0; i<1000; i++);					//delay 1ms
	OLED_Command(OLED_DISPLAYON);
//	for(int i=0; i<10000; i++);					//delay 10ms

}

static void OLED_Config_Display (void)
{

	OLED_Reset();

	// Turn the OLED Display off
	OLED_Command(OLED_DISPLAYOFF);

	// Configure the display for 128x64 pixels, KS0108 mode
	OLED_Command(OLED_SETDISPLAYCLOCKDIV);
	OLED_Command(0x80);
	OLED_Command(OLED_SETMULTIPLEX);
	OLED_Command(OLED_HEIGHT-1);				// LCD Height
	OLED_Command(OLED_SETDISPLAYOFFSET);
	OLED_Command(0x0);
	OLED_Command(OLED_SETSTARTLINE | 0x0);
	OLED_Command(OLED_CHARGEPUMP);
	OLED_Command(0x14);							// Use 3.3V supply to generate high voltage supply
	OLED_Command(OLED_MEMORYMODE);
	OLED_Command(0x00);
	OLED_Command(OLED_SEGREMAP | 0x1);
	OLED_Command(OLED_COMSCANDEC);
	OLED_Command(OLED_SETCOMPINS);
	OLED_Command(0x12);
	OLED_Command(OLED_SETCONTRAST);
	OLED_Command(0xCF);
	OLED_Command(OLED_SETPRECHARGE);
	OLED_Command(0xF1);
	OLED_Command(OLED_SETVCOMDETECT);
	OLED_Command(0x40);
	OLED_Command(OLED_DISPLAYALLON_RESUME);
	OLED_Command(OLED_NORMALDISPLAY);
	OLED_Command(OLED_DEACTIVATE_SCROLL);

	OLED_Command(OLED_COLUMNADDR);
	OLED_Command(0);
	OLED_Command(OLED_WIDTH-1);
	OLED_Command(OLED_PAGEADDR);
	OLED_Command(0);
	OLED_Command(OLED_HEIGHT/8-1);

	// Turn the OLED display on!
	OLED_Command(OLED_DISPLAYON);

	OLED_Command(OLED_SETLOWCOLUMN | 0x0);  // low col = 0
	OLED_Command(OLED_SETHIGHCOLUMN | 0x0);  // hi col = 0
	OLED_Command(OLED_SETSTARTLINE | 0x0); // line #0

}


static int OLED_Render_Char (uint8_t X_axis, uint8_t Y_axis, uint8_t SC, int8_t String, uint8_t Scale)
{

	uint8_t px, py;
	uint16_t start_pos;

	if ((X_axis >= OLED_WIDTH) || (Y_axis >= OLED_HEIGHT))
	{
		return 1;
	}

	if (String > 127)
	{
		return 2;
	}

	if (Scale > 3)
	{
		return 3;
	}

	start_pos = ((uint8_t)String) * 7;			// Characters have a 7 row offset
	for (px = 0; px < 5; px++)
	{
		for (py = 0; py < 7; py++)
		{
			if ((font5x7[start_pos + py] >> (7 - px)) & 1)
			{
				switch (Scale)
				{
				case 3:
					OLED_Set_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale),  SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale),  SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+2, Y_axis+(py*Scale),  SC);
					OLED_Set_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale)+1, SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale)+1, SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+2, Y_axis+(py*Scale)+1, SC);
					OLED_Set_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale)+2, SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale)+2, SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+2, Y_axis+(py*Scale)+2, SC);
					break;
				case 2:
					OLED_Set_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale),  SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale),  SC);
					OLED_Set_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale)+1, SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale)+1, SC);
					break;
				case 1:
				default:
					OLED_Set_Pixel(X_axis+px, Y_axis+py, SC);
					break;
				}
			}
		}
	}
	return 0;
}

void OLED_Init(void)
{
	//Initialize I2C module
	BOARD_InitPins();
	BOARD_InitBootClocks();

	BOARD_I2C_ConfigurePins();

	I2C_InitModule();


	/*Give the display a reset*/
	OLED_Reset();

	/* Clear the framebuffer*/
	OLED_Clear();

	/*Configure the OLED display controller*/
	OLED_Config_Display();

	//Configure start message with roll ON
	isInit = true;
	screenString = "WELCOME!";
	toggleRoll();

	OLEDtimerClbID = Timer_AddCallback(rollCLB, 15, false); // 15 es bastante rapido.
}

void OLED_Refresh(void)
{

	OLED_Command(0xb0);
	OLED_Command(((0&0xf0)>>4) | 0x10);
	OLED_Command((0&0x0f) | 0x01);

	OLED_Data(&OLED_Buffer[0]);

}

//todo: resetear el buffer temporal tambien.
void OLED_Clear(void)
{
	memset(OLED_Buffer, 0, sizeof(OLED_Buffer));
}

void OLED_Fill(uint8_t Pattern)
{

	memset(OLED_Buffer, Pattern, sizeof(OLED_Buffer));

}

void OLED_Display_Mode (uint8_t Mode)
{

	if (Mode)
	{
		OLED_Command(OLED_INVERTDISPLAY);
	}
	else
	{
		OLED_Command(OLED_NORMALDISPLAY);
	}

}

void OLED_Set_Pixel (uint8_t X_axis, uint8_t Y_axis, uint8_t SC)
{

	if((X_axis >= OLED_WIDTH) || (Y_axis >= OLED_HEIGHT))
	{
		//Do nothing
	}
	else
	{
		switch(SC)
		{
			case kOLED_Pixel_Clear:
				OLED_Buffer [X_axis + (Y_axis / 8) * OLED_WIDTH] &= ~(1 << (Y_axis & 7));
				break;
			case kOLED_Pixel_Set:
				OLED_Buffer [X_axis + (Y_axis / 8) * OLED_WIDTH] |= (1 << (Y_axis & 7));
				break;
		}
	}
}

void OLED_Set_Scroll_Pixel (uint8_t X_axis, uint8_t Y_axis, uint8_t SC)
{
	int page = 0;

	if (Y_axis >= (8))
	{
		page++;
	}

	switch(SC)
	{
		case kOLED_Pixel_Clear:
			OLED_Scroll_Buffer[page][X_axis] &= ~(1 << (Y_axis & 7));
			break;
		case kOLED_Pixel_Set:
			OLED_Scroll_Buffer[page][X_axis]|= (1 << (Y_axis & 7));
			break;
	}
}

static int OLED_Render_Scroll_Char(uint8_t X_axis, uint8_t Y_axis, uint8_t SC, int8_t String, uint8_t Scale)
{

	Y_axis /= 8;
	uint8_t px, py;
	uint16_t start_pos;

	if ((X_axis >= OLED_WIDTH * 8))//|| (Y_axis >= 16))
	{
		return 1;
	}

	start_pos = ((uint8_t)String) * 7;			// Characters have a 7 row offset
	for (px = 0; px < 5; px++)
	{
		for (py = 0; py < 7; py++)
		{
			if ((font5x7[start_pos + py] >> (7 - px)) & 1)
			{
				switch (Scale)
				{
				case 3:
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale),  SC);
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale),  SC);
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale)+2, Y_axis+(py*Scale),  SC);
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale)+1, SC);
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale)+1, SC);
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale)+2, Y_axis+(py*Scale)+1, SC);
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale)+2, SC);
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale)+2, SC);
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale)+2, Y_axis+(py*Scale)+2, SC);
					break;
				case 2:
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale),  SC);
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale),  SC);
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale)+1, SC);
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale)+1, SC);
					break;
				case 1:
				default:
					OLED_Set_Scroll_Pixel(X_axis + px, Y_axis + py, SC);
					break;
				}
			}
		}
	}
	return 0;
}



void OLED_Set_Text (uint8_t X_axis, uint8_t Y_axis, uint8_t SC, char* String, uint8_t Scale)
{
	uint16_t Cont;
	uint16_t xscaled;

	int strLength = strlen(String);
	if (strLength > 12)
	{
		roll = true;
	}

	for (Cont = 0; String[Cont] != '\0'; Cont++)
	{
		// Catch overflow when scaling!
		xscaled = X_axis + (Cont * 5 * Scale);

		OLED_Render_Scroll_Char(xscaled, Y_axis, SC, String[Cont], Scale);

		if ((xscaled > OLED_WIDTH))
		{
			//Do nothing
		}
		else
		{
			OLED_Render_Char(xscaled, Y_axis, SC, String[Cont], Scale);
		}
	}
}

void OLED_Copy_Image(const uint8_t *Img, uint16_t size)
{

	uint16_t CpyBuffer;

	OLED_Clear();

	for(CpyBuffer = 0; CpyBuffer < size - 1; CpyBuffer++)
	{
		OLED_Buffer[CpyBuffer] = *(Img + CpyBuffer);
	}
}

bool OLEDisInit()
{
	return isInit;
}

void OLED_write_Text(uint8_t X_axis, uint8_t Y_axis, char* String)
{
	int strLength = strlen(String);

	if (strLength > 12)
	{
		roll = true;
	}

	screenString = String;

	OLED_Set_Text(X_axis, Y_axis, kOLED_Pixel_Set, screenString, 2);
}

static void rollCLB(void)
{
	static bool start = true;
	if (start)
	{
		OLED_Set_Text(24, 32, kOLED_Pixel_Set, screenString, 2);
		OLED_Refresh();
		start = false;
	}

	if(roll)
	{
		shiftPageLeft(4, 2);
		OLED_Refresh();
	}
	else
	{
		OLED_Refresh();
	}
}


static void shiftPageLeft(uint8_t page, uint8_t scale)
{
    static int index = 0;
	int startIndex = page * OLED_WIDTH;

	int strLength = strlen(screenString);
	//todo: cambiar logica para que el scroll sea completo.
	if (index > (6 * scale * strLength))
	{
		index = 0;
	}


	for (int i = 0; i < OLED_WIDTH; i++)
	{
		OLED_Buffer[startIndex + i] = OLED_Scroll_Buffer[0][i + index];
	}

	startIndex = (page + 1) * OLED_WIDTH;
	for (int i = 0; i < OLED_WIDTH; i++)
	{
		OLED_Buffer[startIndex + i] = OLED_Scroll_Buffer[1][i + index];
	}

	index++;

}

static void toggleRoll()
{
	roll = !roll;
}



