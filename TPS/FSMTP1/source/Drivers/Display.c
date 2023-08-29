/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "Display.h"
#include "gpio.h"
#include "board.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/



/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void Display_Init(void)
{
	gpioMode(PIN_SEG_A,OUTPUT_PULLDOWN);
	gpioMode(PIN_SEG_B,OUTPUT_PULLDOWN);
	gpioMode(PIN_SEG_C,OUTPUT_PULLDOWN);
	gpioMode(PIN_SEG_D,OUTPUT_PULLDOWN);
	gpioMode(PIN_SEG_E,OUTPUT_PULLDOWN);
	gpioMode(PIN_SEG_F,OUTPUT_PULLDOWN);
	gpioMode(PIN_SEG_G,OUTPUT_PULLDOWN);
	gpioMode(PIN_SEG_DP,OUTPUT_PULLDOWN);

	gpioMode(PIN_SEL0,OUTPUT);
	gpioMode(PIN_SEL1,OUTPUT);

	writeDigit(-1,0);
	writeDigit(-1,1);
	writeDigit(-1,2);
	writeDigit(-1,3);

}
void updateDisplay(int number){
	static int dp_num[4]={0,0,0,0};
	static int dp_digit=0;

	if (number==-1)
	{
		writeDigit(dp_num[dp_digit],dp_digit);
		dp_digit++;
		dp_digit=dp_digit%4;
	}
	else
		{
			int temp = number;
			for (int i = 3; i >= 0; i--)
			{
				dp_num[i] = temp % 10;
				temp /= 10;
			}
		}
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


void writeDigit (int number,uint8_t digit)
{
	if (number>10)
		number=number%10;
	switch (digit)
		{
			case 0:
				gpioWrite(PIN_SEL0,0);
				gpioWrite(PIN_SEL1,0);
				break;
			case 1:
				gpioWrite(PIN_SEL1,0);
				gpioWrite(PIN_SEL0,1);
				break;
			case 2:
				gpioWrite(PIN_SEL1,1);
				gpioWrite(PIN_SEL0,0);
				break;
			case 3:
				gpioWrite(PIN_SEL0,1);
				gpioWrite(PIN_SEL1,1);
				break;
		}
	switch (number)
	{
		case -1:
			gpioWrite(PIN_SEG_A,0);
			gpioWrite(PIN_SEG_B,0);
			gpioWrite(PIN_SEG_C,0);
			gpioWrite(PIN_SEG_D,0);
			gpioWrite(PIN_SEG_E,0);
			gpioWrite(PIN_SEG_F,0);
			gpioWrite(PIN_SEG_G,0);
			gpioWrite(PIN_SEG_DP,0);
			break;
		case 0:
			gpioWrite(PIN_SEG_A,1);
			gpioWrite(PIN_SEG_B,1);
			gpioWrite(PIN_SEG_C,1);
			gpioWrite(PIN_SEG_D,1);
			gpioWrite(PIN_SEG_E,1);
			gpioWrite(PIN_SEG_F,1);
			gpioWrite(PIN_SEG_G,0);
			gpioWrite(PIN_SEG_DP,0);
			break;
		case 1:
			gpioWrite(PIN_SEG_A,0);
			gpioWrite(PIN_SEG_B,1);
			gpioWrite(PIN_SEG_C,1);
			gpioWrite(PIN_SEG_D,0);
			gpioWrite(PIN_SEG_E,0);
			gpioWrite(PIN_SEG_F,0);
			gpioWrite(PIN_SEG_G,0);
			gpioWrite(PIN_SEG_DP,0);
			break;
		case 2:
			gpioWrite(PIN_SEG_A,1);
			gpioWrite(PIN_SEG_B,1);
			gpioWrite(PIN_SEG_C,0);
			gpioWrite(PIN_SEG_D,1);
			gpioWrite(PIN_SEG_E,1);
			gpioWrite(PIN_SEG_F,0);
			gpioWrite(PIN_SEG_G,1);
			gpioWrite(PIN_SEG_DP,0);
			break;
		case 3:
			gpioWrite(PIN_SEG_A,1);
			gpioWrite(PIN_SEG_B,1);
			gpioWrite(PIN_SEG_C,1);
			gpioWrite(PIN_SEG_D,1);
			gpioWrite(PIN_SEG_E,0);
			gpioWrite(PIN_SEG_F,0);
			gpioWrite(PIN_SEG_G,1);
			gpioWrite(PIN_SEG_DP,0);
			break;
		case 4:
			gpioWrite(PIN_SEG_A,0);
			gpioWrite(PIN_SEG_B,1);
			gpioWrite(PIN_SEG_C,1);
			gpioWrite(PIN_SEG_D,0);
			gpioWrite(PIN_SEG_E,0);
			gpioWrite(PIN_SEG_F,1);
			gpioWrite(PIN_SEG_G,1);
			gpioWrite(PIN_SEG_DP,0);
			break;
		case 5:
			gpioWrite(PIN_SEG_A,1);
			gpioWrite(PIN_SEG_B,0);
			gpioWrite(PIN_SEG_C,1);
			gpioWrite(PIN_SEG_D,1);
			gpioWrite(PIN_SEG_E,0);
			gpioWrite(PIN_SEG_F,1);
			gpioWrite(PIN_SEG_G,1);
			gpioWrite(PIN_SEG_DP,0);
			break;
		case 6:
			gpioWrite(PIN_SEG_A,1);
			gpioWrite(PIN_SEG_B,0);
			gpioWrite(PIN_SEG_C,1);
			gpioWrite(PIN_SEG_D,1);
			gpioWrite(PIN_SEG_E,1);
			gpioWrite(PIN_SEG_F,1);
			gpioWrite(PIN_SEG_G,1);
			gpioWrite(PIN_SEG_DP,0);
			break;
		case 7:
			gpioWrite(PIN_SEG_A,1);
			gpioWrite(PIN_SEG_B,1);
			gpioWrite(PIN_SEG_C,1);
			gpioWrite(PIN_SEG_D,0);
			gpioWrite(PIN_SEG_E,0);
			gpioWrite(PIN_SEG_F,0);
			gpioWrite(PIN_SEG_G,0);
			gpioWrite(PIN_SEG_DP,0);
			break;
		case 8:
			gpioWrite(PIN_SEG_A,1);
			gpioWrite(PIN_SEG_B,1);
			gpioWrite(PIN_SEG_C,1);
			gpioWrite(PIN_SEG_D,1);
			gpioWrite(PIN_SEG_E,1);
			gpioWrite(PIN_SEG_F,1);
			gpioWrite(PIN_SEG_G,1);
			gpioWrite(PIN_SEG_DP,0);
			break;
		case 9:
			gpioWrite(PIN_SEG_A,1);
			gpioWrite(PIN_SEG_B,1);
			gpioWrite(PIN_SEG_C,1);
			gpioWrite(PIN_SEG_D,1);
			gpioWrite(PIN_SEG_E,0);
			gpioWrite(PIN_SEG_F,1);
			gpioWrite(PIN_SEG_G,1);
			gpioWrite(PIN_SEG_DP,0);
			break;
		case 10:
			gpioWrite(PIN_SEG_A,0);
			gpioWrite(PIN_SEG_B,0);
			gpioWrite(PIN_SEG_C,0);
			gpioWrite(PIN_SEG_D,0);
			gpioWrite(PIN_SEG_E,0);
			gpioWrite(PIN_SEG_F,0);
			gpioWrite(PIN_SEG_G,0);
			gpioWrite(PIN_SEG_DP,1);
			break;
	}

}

/*******************************************************************************
 ******************************************************************************/
