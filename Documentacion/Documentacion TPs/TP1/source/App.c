/***************************************************************************/ /**
  @file     App.c
  @brief    Application functions
  @author   Grupo 2
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "queue.h"
#include "fsm.h"
#include "fsm_table.h"
#include "data_base.h"


#include "Led.h"
#include "timer.h"
#include "seven_seg_display.h"
#include "encoder.h"
#include "button.h"
#include "lector.h"
#include "MplxLed.h"
#include "board.h"

#ifdef TESTPOINT
#include "gpio.h"
#define __TP__ PORTNUM2PIN(PC,4)
#endif

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define TIMER_TIMEOUT 60000 //10 SEGS
/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/**
 * @brief function that fills the event queue with events from hardware.
 */
static void fillQueue(void);

static void timeOutCallback(void);

static STATE *currentState;

static int userTimeOutTimerID;
/*******************************************************************************
 *******************************************************************************
                        FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init(void)
{
	initializeDataBase();
  initQueue();
  SevenSegDisplay_Init();
  Led_Init();
  Timer_Init();
  Lector_Init();
  MplxLed_Init();
  Encoder_Init();
  buttonsInit();
  buttonConfiguration(ENCODER_SW, LKP, 20); //20*50=1seg
  currentState = FSM_GetInitState();
  FSM_StartInitState();
  userTimeOutTimerID = Timer_AddCallback(&timeOutCallback, TIMER_TIMEOUT, false);
	#ifdef TESTPOINT
	gpioMode(__TP__,OUTPUT);
	#endif
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run(void)
{
  fillQueue();
  EventType event = getEvent();
  if (event != NONE_EV)
  {
    currentState = fsm(currentState, event);
  }
}

void fillQueue(void)
{

  if (Lector_Event())
  {
    Timer_Reset(userTimeOutTimerID);
    emitEvent(CARD_SWIPE_EV);
  }
  uint8_t move = Encoder_GetMove();
  if (move)
  {
	Timer_Reset(userTimeOutTimerID);
    if (move == 1)
    {
      emitEvent(ENCODER_RIGHT_EV);
    }
	 else
	{
		 emitEvent(ENCODER_LEFT_EV);
	}
  }
  if (wasTap(ENCODER_SW)) 
  {
	  Timer_Reset(userTimeOutTimerID);
	  emitEvent(PRESS_EV);
  }
  else if (wasLkp(ENCODER_SW))
  {
	  Timer_Reset(userTimeOutTimerID);
	  emitEvent(LKP_EV);
  }
}

void timeOutCallback(void)
{
	emitEvent(TIMEOUT_EV);
}
/*******************************************************************************
 *******************************************************************************/
