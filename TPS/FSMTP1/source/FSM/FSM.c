/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Bruno Di Sanzo
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "FSM.h"
#include "queue.h"


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

/**
 * @brief
 * @param
 * @param
 */
void fsm (bool timer_sec, bool timer_min)
{
	switch(states)
	{
	case ID_ENTRY:

		if(encoder) //aca habria que llamar a alguna funcion que devuelva que se esta ingresando la contra
			states = ENCODER_ENTRY;
		else if (card)
			states = CARD_ENTRY;

		break;

	case ENCODER_ENTRY:
			if(id_encoder == ENCODER)//hay que ver como se hace esto, y como es la base de datos
				states = PIN_ENTRY;
			else
				states = RED_LED_ON;
		break;

	case CARD_ENTRY:
			if(id_card == CARD)//hay que ver como se hace esto, y como es la base de datos
				states = PIN_ENTRY;

			else
				states = RED_LED_ON;
		break;

	case PIN_ENTRY:
			if()
		break;

	case RED_LED_ON:

			break;

	case GREEN_LED_ON:

			break;

	case BRIGHTNESS:

			break;

	case WRONG_ID:

			break;

	default:
		break;
	}

}



/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/




/*******************************************************************************
 ******************************************************************************/
