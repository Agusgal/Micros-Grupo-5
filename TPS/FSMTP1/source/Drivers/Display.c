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
#include <string.h>
#include "Systick.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define PRINT_ARRAY_LENGTH 30
#define SPARE_SPACE_4_SCROLL -3
#define SCROLL_TIMER_LIMIT 50
#define ABS(x) ((x) < 0 ? -(x) : (x))

enum scroll_type {no_scroll,scroll_left,scroll_right};

const unsigned char sevseg_digits_code[75]= {
/*  0     1     2     3     4     5     6     7     8     9     :     ;     */
    0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B, 0x00, 0x00,
/*  <     =     >     ?     @     A     B     C     D     E     F     G     */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47, 0x5E,
/*  H     I     J     K     L     M     N     O     P     Q     R     S     */
    0x37, 0x06, 0x3C, 0x57, 0x0E, 0x55, 0x15, 0x1D, 0x67, 0x73, 0x05, 0x5B,
/*  T     U     V     W     X     Y     Z     [     \     ]     ^     _     */
    0x0F, 0x3E, 0x1C, 0x5C, 0x13, 0x3B, 0x6D, 0x00, 0x00, 0x00, 0x00, 0x00,
/*  `     a     b     c     d     e     f     g     h     i     j     k     */
    0x00, 0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47, 0x5E, 0x37, 0x06, 0x3C, 0x57,
/*  l     m     n     o     p     q     r     s     t     u     v     w     */
    0x0E, 0x55, 0x15, 0x1D, 0x67, 0x73, 0x05, 0x5B, 0x0F, 0x3E, 0x1C, 0x5C,
/*  x     y     z     */
    0x13, 0x3B, 0x6D };
/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void sevenSegmentDecoder (uint8_t code);

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
	SysTick_Reg_Callback(muxDisplay,2000);

}

void writeMessage(char * message, bool scroll)
{
	updateDisplay(message);
	if (scroll)
		updateDisplay("CONT_SCR_R");
}
void pauseMessage()
{
	updateDisplay("");
}
void pauseScroll()
{
	updateDisplay("PAUSE_SCR");
}
void continueScroll(char scroll)
{
	if ((scroll=='R') | (scroll=='r') )
		updateDisplay("CONT_SCR_R");
	if ((scroll=='L') | (scroll=='l') )
		updateDisplay("CONT_SCR_l");
}
void ScrollRightOnce()
{
	updateDisplay("SCR_R");
}
void ScrollLeftOnce()
{
	updateDisplay("SCR_L");
}
void toggleScroll()
{
	updateDisplay("TOGGLE_SCR");
}
void muxDisplay()
{
	updateDisplay("MUX");
}
/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


void writeDigit (int character,uint8_t digit)
{
	uint8_t code;
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

	if ((character > (unsigned char)'z') || (character==' ') || (character==0))
		code = 0x00;
	else if (character == '.')
		code = 0x80;
	else code = sevseg_digits_code[character - '0'];

	sevenSegmentDecoder(code);
}


void sevenSegmentDecoder (uint8_t code)
{
		gpioWrite(PIN_SEG_DP,(code >> 7) & 1);
		gpioWrite(PIN_SEG_A,(code >> 6) & 1);
		gpioWrite(PIN_SEG_B,(code >> 5) & 1);
		gpioWrite(PIN_SEG_C,(code >> 4) & 1);
		gpioWrite(PIN_SEG_D,(code >> 3) & 1);
		gpioWrite(PIN_SEG_E,(code >> 2) & 1);
		gpioWrite(PIN_SEG_F,(code >> 1) & 1);
		gpioWrite(PIN_SEG_G,(code >> 0) & 1);
}


void updateDisplay(char * txt)
{
	static char txt2print[PRINT_ARRAY_LENGTH]={0};
	static uint8_t input_txt_length=0;
	static uint8_t mux_digit=0;
	static int scroll_index=0;
	static uint8_t scroll_type=no_scroll;
	static uint8_t scroll_timer=0;

	if (scroll_type==scroll_right)
		{
			scroll_timer++;
			if (scroll_timer==SCROLL_TIMER_LIMIT)
			{
				scroll_index++;
				scroll_timer=0;
			}
		}
		else if (scroll_type==scroll_left)
		{
			scroll_timer++;
			if (scroll_timer==SCROLL_TIMER_LIMIT)
			{
				scroll_index--;
				scroll_timer=0;
			}
		}

	if (!strcmp(txt,"CONT_SCR_R"))
		scroll_type=scroll_right;

	else if (!strcmp(txt,"CONT_SCR_L"))
		scroll_type=scroll_left;

	else if (!strcmp(txt,"SCR_R"))
		scroll_index++;

	else if (!strcmp(txt,"SCR_L"))
		scroll_index--;

	else if (!strcmp(txt,"PAUSE_SCR"))
		scroll_type=no_scroll;
	else if (!strcmp(txt,"TOGGLE_SCR"))
	{
		if (scroll_type==no_scroll)
			scroll_type=scroll_right;
		else
			scroll_type=no_scroll;
	}
	else if (!strcmp(txt,"MUX"))
	{
		if ((scroll_index==input_txt_length) && (scroll_type==scroll_right))
			scroll_index=SPARE_SPACE_4_SCROLL;

		if ((scroll_index==SPARE_SPACE_4_SCROLL-1) && (scroll_type==scroll_left))
			scroll_index=input_txt_length-1;

		if ((scroll_index+mux_digit)<0)
			writeDigit(0,mux_digit);
		else
			writeDigit(txt2print[scroll_index+mux_digit],mux_digit);
		mux_digit++;
		if (mux_digit==4)
			mux_digit=0;
	}

	else
	{
		int i=0;
		input_txt_length=0;
		while (txt[i]!='\0')
		{
			input_txt_length++;
			txt2print[i]=txt[i];
			i++;
		}
		while (i<PRINT_ARRAY_LENGTH)
		{
			txt2print[i]=0;
			i++;
		}
		scroll_index=0;
		scroll_type=no_scroll;
		scroll_timer=0;
	}

}

char * int2str (int num)
{
	static char string [20];
	for (int i=0;i<20;i++)
		string[i]=0;
	int numDigits = 0;
	int temp = num;

	// Count the number of digits in the number
	while (temp != 0)
	{
		temp /= 10;
		numDigits++;
	}

	// Create an array to hold the digits
	temp = num;

	// Extract the digits and store them in the array
	for (int i = numDigits - 1; i >= 0; i--) {
		string[i] = temp % 10;
		string[i]+='0';
		temp /= 10;
	}
	return string;

}

/*******************************************************************************
 ******************************************************************************/
