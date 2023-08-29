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
#define PRINT_ARRAY_SIZE 20
#define SCROLL_TIMER_LIMIT 100
#define BACKSPACE4SCROLL -3
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
void updateDisplay(string txt)
{
	static char txt_print[PRINT_ARRAY_SIZE]={0};
	static int mux_digit=0;
	static int scroll_index=0;
	static char txt_length=0;
	static char scroll_timer=0;
	static bool scroll_left=FALSE;
	static bool scroll_right=FALSE;

	if (scroll_left | scroll_right)
		scroll_timer++;

	if (txt=="CONT_SCR_R")
	{
		scroll_right=TRUE;
		scroll_left=FALSE;
	}
	else if (txt=="CONT_SCR_L")
	{
		scroll_left=TRUE;
		scroll_right=FALSE;
	}
	else if (txt=="SCR_R")
	{
		scroll_left=FALSE;
		scroll_right=FALSE;
		scroll_index++;
		if (scroll_index==txt_length)
			scroll_index=BACKSPACE4SCROLL;
	}
	else if (txt=="SCR_L")
		{
			scroll_left=FALSE;
			scroll_right=FALSE;
			scroll_index--;
			if (scroll_index==BACKSPACE4SCROLL)		// bs-1
				scroll_index=txt_length-1;
		}
	else if (txt=="PAUSE_SCROLL")
	{
		scroll_left=FALSE;
		scroll_right=FALSE;
		scroll_timer=0;
	}
	else if (txt=="MUX")
	{
		int dig2print=scroll_index+mux_digit;
		if ((dig2print<0) | (dig2print>=0))
			writeDigit(0,mux_digit);
		else
			writeDigit(txt_print[dig2print],mux_digit);
		mux_digit++;
		if (mux_digit==4)
			mux_digit=0;
	}
	else
	{
		int i=0;
		while (txt[i])!='\0')
		{
			txt_length++;
			txt_print[i]=txt[i];
		}
		while (i<PRINT_ARRAY_SIZE)
		{
			txt_print[i]=0;
		}
		scroll_left=FALSE;
		scroll_right=FALSE;
		scroll_index=0;
		scroll_timer=0;
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
		case 0:
			gpioWrite(PIN_SEG_A,0);
			gpioWrite(PIN_SEG_B,0);
			gpioWrite(PIN_SEG_C,0);
			gpioWrite(PIN_SEG_D,0);
			gpioWrite(PIN_SEG_E,0);
			gpioWrite(PIN_SEG_F,0);
			gpioWrite(PIN_SEG_G,0);
			gpioWrite(PIN_SEG_DP,0);
			break;
		case '0':
			gpioWrite(PIN_SEG_A,1);
			gpioWrite(PIN_SEG_B,1);
			gpioWrite(PIN_SEG_C,1);
			gpioWrite(PIN_SEG_D,1);
			gpioWrite(PIN_SEG_E,1);
			gpioWrite(PIN_SEG_F,1);
			gpioWrite(PIN_SEG_G,0);
			gpioWrite(PIN_SEG_DP,0);
			break;
		case '1':
			gpioWrite(PIN_SEG_A,0);
			gpioWrite(PIN_SEG_B,1);
			gpioWrite(PIN_SEG_C,1);
			gpioWrite(PIN_SEG_D,0);
			gpioWrite(PIN_SEG_E,0);
			gpioWrite(PIN_SEG_F,0);
			gpioWrite(PIN_SEG_G,0);
			gpioWrite(PIN_SEG_DP,0);
			break;
		case '2':
			gpioWrite(PIN_SEG_A,1);
			gpioWrite(PIN_SEG_B,1);
			gpioWrite(PIN_SEG_C,0);
			gpioWrite(PIN_SEG_D,1);
			gpioWrite(PIN_SEG_E,1);
			gpioWrite(PIN_SEG_F,0);
			gpioWrite(PIN_SEG_G,1);
			gpioWrite(PIN_SEG_DP,0);
			break;
		case '3':
			gpioWrite(PIN_SEG_A,1);
			gpioWrite(PIN_SEG_B,1);
			gpioWrite(PIN_SEG_C,1);
			gpioWrite(PIN_SEG_D,1);
			gpioWrite(PIN_SEG_E,0);
			gpioWrite(PIN_SEG_F,0);
			gpioWrite(PIN_SEG_G,1);
			gpioWrite(PIN_SEG_DP,0);
			break;
		case '4':
			gpioWrite(PIN_SEG_A,0);
			gpioWrite(PIN_SEG_B,1);
			gpioWrite(PIN_SEG_C,1);
			gpioWrite(PIN_SEG_D,0);
			gpioWrite(PIN_SEG_E,0);
			gpioWrite(PIN_SEG_F,1);
			gpioWrite(PIN_SEG_G,1);
			gpioWrite(PIN_SEG_DP,0);
			break;
		case '5':
			gpioWrite(PIN_SEG_A,1);
			gpioWrite(PIN_SEG_B,0);
			gpioWrite(PIN_SEG_C,1);
			gpioWrite(PIN_SEG_D,1);
			gpioWrite(PIN_SEG_E,0);
			gpioWrite(PIN_SEG_F,1);
			gpioWrite(PIN_SEG_G,1);
			gpioWrite(PIN_SEG_DP,0);
			break;
		case '6':
			gpioWrite(PIN_SEG_A,1);
			gpioWrite(PIN_SEG_B,0);
			gpioWrite(PIN_SEG_C,1);
			gpioWrite(PIN_SEG_D,1);
			gpioWrite(PIN_SEG_E,1);
			gpioWrite(PIN_SEG_F,1);
			gpioWrite(PIN_SEG_G,1);
			gpioWrite(PIN_SEG_DP,0);
			break;
		case '7':
			gpioWrite(PIN_SEG_A,1);
			gpioWrite(PIN_SEG_B,1);
			gpioWrite(PIN_SEG_C,1);
			gpioWrite(PIN_SEG_D,0);
			gpioWrite(PIN_SEG_E,0);
			gpioWrite(PIN_SEG_F,0);
			gpioWrite(PIN_SEG_G,0);
			gpioWrite(PIN_SEG_DP,0);
			break;
		case '8':
			gpioWrite(PIN_SEG_A,1);
			gpioWrite(PIN_SEG_B,1);
			gpioWrite(PIN_SEG_C,1);
			gpioWrite(PIN_SEG_D,1);
			gpioWrite(PIN_SEG_E,1);
			gpioWrite(PIN_SEG_F,1);
			gpioWrite(PIN_SEG_G,1);
			gpioWrite(PIN_SEG_DP,0);
			break;
		case '9':
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
