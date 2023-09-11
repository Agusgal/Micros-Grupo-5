/***************************************************************************//**
  @file     FSM.c
  @brief    fsm functions.
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


#include "FSM.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void pass(void);


/*******************************************************************************
 * FSM TABLE
 ******************************************************************************/


/*
 * Formato:
 *
 * state stateName[]= {punteroFuncion, EVENTO_EV, nombreSiguienteEstado}
 *
 */




/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/**
 * @brief determines next state and executes transition functions based on current event, current state and fsm table.
 * @param p_state: pointer to current state inside the fsm state table.
 * @param curr_event: latest event, it came from the event queue.
 * @return pointer to the next state following the logic of the state table.
 */
state* fsm_dispatcher(state* p_state, Event_Type curr_event)
{
	bool flag = 1;

	while(flag)
	{
		if(p_state->event == curr_event || p_state->event == END_TABLE)
		{
			flag=0;
		}
		else
		{
			p_state++;
		}
	}

	(*p_state->funct)();

	p_state = p_state->next_state;

	return p_state;
}

//todo: agregar estado inicial
/**
 * @brief returns initial state of the fsm, gets called at the beginning of the program.
 * @return pointer to the initial state of the fsm.
 */
state* get_initial_state()
{

}

//todo agregar funcion de configuracion inicial
/**
 * @brief function executed at the beginning of the fsm.
 * @return nothing.
 */
void start_fsm()
{

}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/**
 * @brief dummy function.
 * @return nothing.
 */
static void pass(void)
{

}


/*******************************************************************************
 ******************************************************************************/
