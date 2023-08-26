/***************************************************************************//**
  @file     7SegDisplay.c
  @brief    Display configurations
  @author   Grupo 2
 ******************************************************************************/

#include "seven_seg_display.h"
#include "SysTick.h"
#include "gpio.h"
#include "board.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define SCREEN_SIZE 4
#define BACK_BUFFER 50
#define BLINK_TIME 60
#define PERIOD 5
#define MOVE_SPEED 700
#define ANIMATION_SPEED 50

#define NONE	0x00
#define _O				-9
#define _A				-8//(uint8_t)0x01
#define _B				-7//(uint8_t)0x02
#define _C				-6//(uint8_t)0x04
#define _D				-1//(uint8_t)0x08
#define _E				-5//(uint8_t)0x10
#define _F				-4//(uint8_t)0x20
#define _G				-3//(uint8_t)0x40

#define MASK			(uint8_t)0x01

/*************************************************
 *  	LOCAL FUNCTION DECLARATION
 ************************************************/

void SevenSegDisplay_PISR(void);
bool SevenSegDisplay_PrintCharacter(uint8_t character);
uint8_t SevenSegDisplay_chat2sevseg(char code);

/************************************************
 *  	VARIABLES WITH LOCAL SCOPE
 ************************************************/
								// 0	 1		2	 3	  4		 5	  6	    7  	  8     9
static const uint8_t numbers[] = {0x3F, 0x06, 0x5B,0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

								// A	 b 		c	  d	   E     F	   g	 H		I	 J
static const uint8_t letters[] = {0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x3D, 0x76, 0x30, 0x1E,
								// k	  L	   ~M	 n		o	 p		q	 r		S	 t
								  0x75, 0x38, 0x00, 0x54, 0x3F, 0x73, 0x67, 0x50, 0x6D, 0x78,
								//  u	 ~v	   ~w	 ~X		y	 ~z
								  0x3E, 0x00, 0x00, 0x00, 0x6E, 0x00};

								// '-'     Back  "_"	otros
//static const uint8_t extras[] = {  0x40,   0x18 , 0x08, 0x00};
static const uint8_t extras[] = {0x00, 0x01, 0x02, 0x04, 0x10, 0x20, 0x40, 0x18 , 0x08, 0x00};


static sevenSeg_t screen[BACK_BUFFER+10];
static uint8_t pos = 0;

static pin_t displayPins[SEG_LEN] = {PIN_SEGA, PIN_SEGB, PIN_SEGC, PIN_SEGD,
							 	 	 PIN_SEGE, PIN_SEGF, PIN_SEGG, PIN_SEGDP};

static pin_t selectPins[SEL_LEN] = {PIN_SEL0, PIN_SEL1};

static bright_t brightness = MAX;

static int8_t moves_remainig = 0;
static int16_t moving_counter = 0;
static bool bouncing = false;
static int8_t move_b = 0;
static bool animation = false;
static uint8_t animation_counter = 0;

static uint8_t 	cursor_pos = 0;
static bool 	cursor = false;
/************************************************
 * 		FUNCTION DEFINITION WITH GLOBAL SCOPE
 ************************************************/
bool SevenSegDisplay_Init(void)
{
	static bool isInit = false;
	if(!isInit)
	{
		SysTick_Init();
		uint8_t count;
		for(count=0; count<SEG_LEN ;count++)
		{
			gpioMode(displayPins[count], OUTPUT);
		}
		for(count=0; count<SEL_LEN ;count++)
		{
			gpioMode(selectPins[count], OUTPUT);
		}
	    //pongo los 2 pin a demultiplexar en 00 para que solo se prenda un 7segmentos
	    gpioWrite (selectPins[0], false);
	    gpioWrite (selectPins[1], false);

	    //sevenSeg_t dummy = {NONE,false,true,0};
	    for(count = 0; count<BACK_BUFFER; count++)
	    {
	    	screen[count].character = 0;
	    	screen[count].blinkCounter = 0;
	    	screen[count].blink = false;
	    	screen[count].blinkState = true;
	    }

	    int systickCallbackID = SysTick_AddCallback(&SevenSegDisplay_PISR, 1); //1 ms
		if (systickCallbackID < 0 ) // Error
		{
			return false;
		}
	//idCounter = 1;
	}
	return true;
}


void SevenSegDisplay_ChangeCharacter(uint8_t screen_char, char new_char)
{
	if (screen_char < SCREEN_SIZE)
	{
		screen[pos + screen_char].character = SevenSegDisplay_chat2sevseg(new_char);
	}
}

bool SevenSegDisplay_BlinkScreen(bool state)
{
	//set all variables on state (true or false)
	uint8_t count;
	for(count=0; count<BACK_BUFFER; count++)
	{
		screen[count].blink = state;
		screen[count].blinkCounter = BLINK_TIME;
		screen[count].blinkState = true;
	}

	return true;
}

bool SevenSegDisplay_BlinkCharacter(uint8_t digit)
{
	//controls if the digit is valid
	if( (digit>=0 && digit <=3) || (digit == RESET_BLINK) )
	{
		//if the digit is valid reset all blink variables
		uint8_t count;
		for(count=0; count<SCREEN_SIZE; count++)
		{
			screen[pos+count].blink=false;
			screen[pos+count].blinkCounter = BLINK_TIME;
			screen[pos+count].blinkState = true;
		}
		//if I want to put a digit to blink
		if(digit!=RESET_BLINK)
		{
			screen[pos+digit].blink=true;
		}
	}else
	{
		//if the digit wasn't validate return false
		return false;
	}
	return true;
}

void SevenSegDisplay_SetBright(bright_t new_bright)
{
	brightness = new_bright;
}

void SevenSegDisplay_WriteBuffer(char new_chars[], uint8_t amount, uint8_t offset)
{
	if((offset+amount) < BACK_BUFFER)
	{
		bouncing=false;
		uint8_t i;
		for(i = 0; i< amount; i++)
		{
			screen[offset+i].character = SevenSegDisplay_chat2sevseg(new_chars[i]);
		}
	}
}

void SevenSegDisplay_WriteBufferAndMove(char new_chars[], uint8_t amount, uint8_t offset, uint8_t move_type)
{
	bouncing = false;
	SevenSegDisplay_WriteBuffer(new_chars, amount, offset);
	switch(move_type)
	{
		case SHIFT_L:
			SevenSegDisplay_Swipe(amount-4);
			break;
		case BOUNCE:
			{
				SevenSegDisplay_Swipe(amount-4);
				bouncing = true;
				move_b = amount - 4;
			}
			break;
		case SHIFT_R:
				SevenSegDisplay_Swipe(-(amount-4));
			break;
		default:
			break;
	}
}

void SevenSegDisplay_Swipe(int8_t moves)
{
	if((pos+moves >= 0) && (pos+moves < BACK_BUFFER-SCREEN_SIZE))
	{
		bouncing = false;
		moves_remainig = moves;
		moving_counter = MOVE_SPEED;
	}
}

void SevenSegDisplay_SetPos(uint8_t new_pos)
{
	if(new_pos < BACK_BUFFER - SCREEN_SIZE )
	{
		moves_remainig = 0;
		pos = new_pos;
	}
}

void SevenSegDisplay_CursorOn(void)
{
	if(!cursor)
	{
		bouncing = false;
		cursor = true;
		cursor_pos = 0;
		SevenSegDisplay_BlinkCharacter(0);
	}
}

void SevenSegDisplay_CursorOff(void)
{
	if(cursor)
	{
		cursor = false;
		cursor_pos = 0;
		SevenSegDisplay_BlinkScreen(false);
	}
}

void SevenSegDisplay_CursorInc(void)
{
	if(cursor)
	{
		if(cursor_pos >= SCREEN_SIZE-1)
		{
			pos += 1;
		}
		else
		{
			cursor_pos++;
		}
		SevenSegDisplay_BlinkCharacter(cursor_pos);
	}
}

void SevenSegDisplay_CursorDec(void)
{
	if(cursor)
	{
		if(cursor_pos == 0)
		{
			pos-=1;
		}
		else
		{
			cursor_pos--;
		}
		SevenSegDisplay_BlinkCharacter(cursor_pos);
	}

}

/**************************************************
 * 			LOCAL FUNCTIONS DEFINITIONS
 **************************************************/

/**
 * @brief print one character on one 7 segments display
 * @param character to print
 * @param array with pins to 7 segments display
 * @return printed succeed
 */
bool SevenSegDisplay_PrintCharacter(uint8_t character)
{
	int count;

	for(count=0; count<SEG_LEN; count++)
	{
		gpioWrite(displayPins[count], (character & (1<<count)) != 0);
	}
	return 0;
}

void SevenSegDisplay_PISR(void)
{
	static uint8_t displayCounter = 0;
	static int8_t  currBright = MAX;
	static uint8_t window = PERIOD;

	uint8_t curr_pos = pos+displayCounter;

	uint8_t dataToPrint = (screen[curr_pos].blinkState && (currBright > 0)) ? screen[curr_pos].character: NONE;

	gpioWrite(selectPins[0], (displayCounter & (1   )) != 0);
	gpioWrite(selectPins[1], (displayCounter & (1<<1)) != 0);
	SevenSegDisplay_PrintCharacter(dataToPrint);

	if(screen[curr_pos].blink)
	{
		if(--screen[curr_pos].blinkCounter == 0)
		{
			screen[curr_pos].blinkCounter = BLINK_TIME;
			screen[curr_pos].blinkState = !screen[curr_pos].blinkState;
		}
	}

	if(moves_remainig != 0)
	{
		if(--moving_counter == 0)
		{
			int8_t i = moves_remainig > 0 ? 1:-1;
			pos += i;
			moves_remainig -= i;
			if(moves_remainig != 0)
			{
				moving_counter = MOVE_SPEED;
			}
			else if(bouncing)
			{
				moves_remainig = -move_b;
				move_b = -move_b;
				moving_counter = MOVE_SPEED;
			}
		}
	}

	if(animation)
	{
		if(--moving_counter <= 0)
		{
			animation_counter++;
			moving_counter = ANIMATION_SPEED;
			SevenSegDisplay_AnimationCircles();
		}
	}

	currBright--;
	if(--window == 0)
	{
		displayCounter++;
		window = PERIOD;
		currBright = brightness;
		if(displayCounter == SCREEN_SIZE)
		{
			displayCounter = 0;
		}

	}

}

void SevenSegDisplay_EraseScreen(void)
{
	bouncing = false;
	for(int i = 0; i<((int)(sizeof(screen)/sizeof(screen[0]))); i++)
	{
		screen[i].character = NONE;
	}
}

uint8_t SevenSegDisplay_chat2sevseg(char code)
{
	if(code >= 0 && code <= 9)
	{
		return numbers[code+0];
	}
	else if(code >= 'A' && code <= 'Z')
	{
		return letters[code-'A'];
	}
	else if(code >= 'a' && code <= 'z')
	{
		return letters[code-'a'];
	}
	else if((int8_t)code < 0 && (int8_t)code > -10)
	{
		return extras[(int8_t)code+9];
	}
	return NONE;
}

void SevenSegDisplay_AnimationCircles(void)
{
	animation = true;

	static char animation_word[12][4]={{_F,_O,_O,_O},
											{_E,_O,_O,_O},
											{_D,_O,_O,_O},
											{_O,_D,_O,_O},
											{_O,_O,_D,_O},
											{_O,_O,_O,_D},
											{_O,_O,_O, _C},
											{_O,_O,_O, _B},
											{_O,_O,_O, _A},
											{_O,_A,_O,_O},
											{_A,_O,_O,_O},};

	if(animation_counter>=12)
	{
		animation_counter=0;
	}
	SevenSegDisplay_WriteBuffer(animation_word[animation_counter], 4,0);

}

void SevenSegDisplay_StopAnimation(void)
{
	animation = false;
}
