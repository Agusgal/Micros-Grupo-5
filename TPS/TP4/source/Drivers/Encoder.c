/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "gpio.h"
#include <stdio.h>
#include <os.h>
#include "board.h"
#include "Encoder.h"
#include "Timers.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define S_TO_US		1000000
#define IDLE 0
#define ENCODER_CALLBACK_PERIOD 20000
#define FIVE_SECOND_COUNTER 5 * S_TO_US / ENCODER_CALLBACK_PERIOD
/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static int encoder = NO_MOVE;
static int encoder_sw = IDLE_;

//Def del timer
static tim_id_t encoder_update_timer;
static tim_id_t encoder_switch_timer;

//Semáforo del encoder
static OS_SEM semEncoder;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


void Encoder_Init(void)
{
	initTimers();
	encoder_update_timer = timerGetId();
	encoder_switch_timer = timerGetId();

	gpioMode(PIN_CH_A,INPUT);
	gpioMode(PIN_CH_B,INPUT);
	gpioMode(PIN_DEC_SW,INPUT);

	//Seteo el timer para que llame periodicamente a encoder_callback con 1ms
	timerStart(encoder_update_timer, TIMER_MS2TICKS(5), TIM_MODE_PERIODIC, Encoder_Update);           //5
	timerStart(encoder_switch_timer, TIMER_MS2TICKS(20), TIM_MODE_PERIODIC, EncoderSwitch_Update);    //20


	//Create semaphore
	OS_ERR os_err;
	OSSemCreate(&semEncoder, "Sem Encoder", 0u, &os_err);
}


OS_SEM* encoderSemPointer()
{
	return &semEncoder;
}


void Encoder_Update(void)
{
	OS_ERR os_err;

	static int state = IDLE;
	bool CH_A = gpioRead(PIN_CH_A);
	bool CH_B = gpioRead(PIN_CH_B);
	encoder = IDLE;

	/*****************************************/
	// Máquina de Estados

	switch (state)
	{

		case IDLE:
			if (!CH_A & CH_B)
				state = ACW1;
			else if (CH_A & !CH_B)
				state = CW1;
			else
				state = IDLE;
			break;
		case ACW1:
			if (CH_A & CH_B)
				state = ACW3;
			else if (!CH_A & !CH_B)
				state = ACW2;
			else
				state = ACW1;
			break;
		case ACW2:
			if (!CH_A & CH_B)
				state = ACW1;
			else if (!CH_A & !CH_B)
				state = ACW2;
			else
				state = ACW3;
			break;
		case ACW3:
			if (CH_A & CH_B)
			{
				state=IDLE;
				encoder= LEFT_TURN;
                OSSemPost(&semEncoder, OS_OPT_POST_1, &os_err);
			}
			else if (!CH_A & !CH_B)
				state = ACW2;
			else
				state = ACW3;
			break;
		case CW1:
			if (!CH_A & !CH_B)
				state = CW2;
			else if (CH_A & CH_B)
				state = CW3;
			else
				state = CW1;
			break;
		case CW2:
			if (CH_A & !CH_B)
				state = CW1;
			else if (!CH_A & !CH_B)
				state = CW2;
			else
				state = CW3;
			break;
		case CW3:
			if (CH_A & CH_B)
			{
				state = IDLE;
				encoder = RIGHT_TURN;
                OSSemPost(&semEncoder, OS_OPT_POST_1, &os_err);
			}
			else if (!CH_A & !CH_B)
				state = CW2;
			else
				state = CW3;
			break;
	}


}

void EncoderSwitch_Update(void)
{
	OS_ERR os_err;

	//Puse este post aca porque sno nunca avanza al evento, Todo: revisar donde ponerlo para señalar mejor eventos encoder.
	OSSemPost(&semEncoder, OS_OPT_POST_1, &os_err);

	static int sw_state = LOW;
	static int duration_counter=0;
	bool sw_Read = gpioRead(PIN_DEC_SW);

	if ((sw_state == LOW) && (sw_Read == LOW))
	{
		sw_state = HIGH;
		encoder_sw = RISING_FLANK;
		duration_counter++;
	}
	else if (sw_state == LOW) // && sw_Read == HIGH pero no hace falta
	{
		encoder_sw = IDLE_;
		duration_counter = 0;
	}
	else if ((sw_state == HIGH) && (sw_Read == LOW))
	{
		duration_counter++;
		if (duration_counter >= FIVE_SECOND_COUNTER)
		{
			encoder_sw = FIVE_SEC_PRESSING;
			OSSemPost(&semEncoder, OS_OPT_POST_1, &os_err);
		}
		else
		{
			encoder_sw = PRESSED;
			OSSemPost(&semEncoder, OS_OPT_POST_1, &os_err);
		}
	}
	else if (sw_state == HIGH)
	{
		sw_state = LOW;
		if (duration_counter >= FIVE_SECOND_COUNTER)
			encoder_sw = FIVE_SEC_PRESS;
		else
			encoder_sw = RELEASED;
		duration_counter = 0;
	}
}

int getEncoderSwitch_State(void)
{
	int aux = encoder_sw;
	encoder_sw = IDLE_;
	return aux;
}

int getEncoder_State(void)
{
	int aux = encoder;
	encoder = NO_MOVE;
	return aux;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



/*******************************************************************************
 ******************************************************************************/
