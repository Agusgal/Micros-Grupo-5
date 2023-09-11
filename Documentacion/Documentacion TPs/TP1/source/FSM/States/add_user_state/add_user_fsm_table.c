#include <stdio.h>
#include "../../fsm.h"
#include "../../../queue.h"
#include "add_user_fsm_table.h"
#include "substates/add_user_main_state.h"
#include "substates/add_user_id_state.h"
#include "substates/add_user_pin_state.h"


/*Foward Declarations*/

extern STATE addUser_card[];
extern STATE addUser_id[];
extern STATE addUser_pin[];

// prototipos
static void do_nothing(void);
static void return_to_last(void);

/*** tablas de estado ***/

/*** Card ***/
STATE addUser_card[]=
{
  	{ENCODER_RIGHT_EV, 	addUser_card,	return_to_last},
    {ENCODER_LEFT_EV, 	addUser_card, 	return_to_last},
    {PRESS_EV, 			addUser_card, 	return_to_last},
	{LKP_EV, 			addUser_card, 	return_to_last},
	{CARD_SWIPE_EV, 	addUser_card, 	addUsr_cardSwipe},
	{ID_OK_EV,			addUser_id,		addUserId_initLogin},
	{FIN_TABLA, 		addUser_card, 	do_nothing}
};

/*** Id ***/

STATE addUser_id[]=
{
	{PRESS_EV, 						addUser_id, 	addUserId_acceptNumber},
	{LKP_EV, 						addUser_id, 	addUserId_confirmID},
	{ENCODER_RIGHT_EV, 				addUser_id, 	addUserId_increaseCurrent},
    {ENCODER_LEFT_EV, 				addUser_id, 	addUserId_decreaseCurrent},
	{RETURN_TO_LAST_STATE_EV, 		addUser_card, 	addUsr_initState},
	{ID_OK_EV, 						addUser_pin, 	addUserPin_initPinInput}, //TODO que updetee el display
	{FIN_TABLA,						addUser_id,		do_nothing},
};

/*** Pin ***/

STATE addUser_pin[] =
{
	//{,pin,},	    
	{PRESS_EV,						addUser_pin, 	addUserPin_acceptNumber},
	{ENCODER_RIGHT_EV,				addUser_pin, 	addUserPin_increaseCurrent}, 
    {ENCODER_LEFT_EV,				addUser_pin, 	addUserPin_decreaseCurrent},
	{LKP_EV,						addUser_pin, 	addUserPin_confirmPin}, 
	{RETURN_TO_LAST_STATE_EV, 		addUser_id, 	addUserId_initLogin},
	{FIN_TABLA, 					addUser_pin, 	do_nothing}
};



//========interfaz=================


STATE *subFSM_GetInitState(void) // cambiar nombre
{
	return(addUser_card);
}

///=========Rutinas de accion===============
void subFSM_StartInitState() // cambiar nombre
{
	addUsr_initState();
}


/*Dummy function*/
static void do_nothing(void)
{
	//take some time off, relax, and KEEP WORKING ////you lazy ass ALU
}


static void return_to_last(void)
{
	emitEvent(RETURN_TO_LAST_STATE_EV);
}