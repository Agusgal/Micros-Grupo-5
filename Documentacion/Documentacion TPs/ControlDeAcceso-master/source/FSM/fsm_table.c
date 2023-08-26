/***************************************************************************/ /**
  @file     fsm_Table.c
  @brief    FSM Table
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdio.h>
#include <States/menu_state.h>
#include <States/welcome_state.h>
#include "fsm.h"
#include "fsm_table.h"
#include "states/id_state.h"
#include "states/pin_state.h"
#include "states/open_state.h"
#include "queue.h"
#include "states/config_device_state.h"
#include "States/config_me_state.h"
#include "States/add_user_state/add_user_state.h"
#include "states/delete_user_state.h"

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static void do_nothing(void);

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
/*Foward Declarations*/

extern STATE welcome[];
extern STATE id[];
extern STATE pin[];
extern STATE menu[];
extern STATE welcome[];
extern STATE cfg_me[];
extern STATE cfg_usr[];
extern STATE cfg_device[];
extern STATE add_user[];
extern STATE open[];
extern STATE delete_user[];


/*** tablas de estado ***/

/*** Welcome ***/

 STATE welcome[]=
{
  	{ENCODER_RIGHT_EV, 	id,			initLogin},
    {ENCODER_LEFT_EV, 	id, 		initLogin},
    {PRESS_EV, 			id, 		initLogin},
	{LKP_EV, 			id, 		initLogin},
	{CARD_SWIPE_EV, 	id, 		cardSwipe},
  	{FIN_TABLA, 		welcome, 	do_nothing}
};


/*** Id ***/

STATE id[]=
{
	{PRESS_EV, 					id, 		id_acceptNumber},
	{LKP_EV, 					id, 		id_confirmID},
	{ENCODER_RIGHT_EV, 			id, 		id_increaseCurrent},
    {ENCODER_LEFT_EV, 			id, 		id_decreaseCurrent},
	{CARD_SWIPE_EV, 			id, 		id_checkCardID},
	{RETURN_TO_LAST_STATE_EV, 	welcome, 	showWelcomeAnimation},
	{ID_OK_EV, 					pin, 		initPinInput}, 
	{TIMEOUT_EV, 				welcome, 	showWelcomeAnimation},
	{FIN_TABLA,					id,			do_nothing},
};

/*** Pin ***/

STATE pin[] =
{
	//{,pin,},	    
	{PRESS_EV,					pin, 		pin_acceptNumber},
	{ENCODER_RIGHT_EV,			pin, 		pin_increaseCurrent}, 
    {ENCODER_LEFT_EV,			pin, 		pin_decreaseCurrent},
	{LKP_EV, 					pin, 		pin_confirmPin}, 
	{PIN_OK_EV, 				menu, 		menu_initState},
	{RETURN_TO_LAST_STATE_EV,	id, 		initLogin},
	{USR_BLOCKED_EV, 			welcome, 	showWelcomeAnimation},
	{TIMEOUT_EV, 				welcome, 	showWelcomeAnimation},
	{FIN_TABLA, 				pin, 		do_nothing}
};

/*** Menu ***/

STATE menu[] =
{	/* Eventos externos */
	{PRESS_EV,							menu, 		menu_selectOption},
	{ENCODER_RIGHT_EV, 					menu, 		menu_nextOption}, 
    {ENCODER_LEFT_EV,					menu, 		menu_previousOption}, 
	/* Eventos de user y admin */ 
	{OPEN_SELECTED_EV,					open,		openDoor},
	{CONFIG_PIN_SELECTED_EV,			cfg_me,		configMe_initState},
	/* Eventos exclusivos de admin*/ 
	{ADD_USER_SELECTED_EV,				add_user,	addUser_initFSM},
    {DELETE_USER_SELECTED_EV,			delete_user,deleteUser_initState},
    {CONFIG_DEVICE_SELECTED_EV,			cfg_device,	initConfigDevice},
	{TIMEOUT_EV, 						welcome, 	showWelcomeAnimation},
	{FIN_TABLA, 						menu, 		do_nothing}
};

/*** Open ***/
STATE open[] =
{
	{TIMEOUT_EV,	welcome,	showWelcomeAnimation},
	{FIN_TABLA, 		open, 		do_nothing}
};

/*** User config me ***/
STATE cfg_me[] = 
{
	{PRESS_EV,					cfg_me, 	configMe_acceptNumber},
	{ENCODER_RIGHT_EV,			cfg_me, 	configMe_increaseCurrent}, 
    {ENCODER_LEFT_EV,			cfg_me, 	configMe_decreaseCurrent},
	{LKP_EV, 					cfg_me, 	configMe_confirmPin}, 
	{RETURN_TO_LAST_STATE_EV, 	menu,		menu_initState},
	{TIMEOUT_EV, 				welcome, 	showWelcomeAnimation},
	{FIN_TABLA, 				cfg_me, 	do_nothing}
};

/*** Config usr ***/
STATE add_user[] = 
{	
	{PRESS_EV, 					add_user, 	addUser_onPress},
	{LKP_EV, 					add_user, 	addUser_onLKP},
	{ENCODER_RIGHT_EV, 			add_user, 	addUser_onEncoderRight},
    {ENCODER_LEFT_EV, 			add_user, 	addUser_onEncoderLeft},
	{CARD_SWIPE_EV, 			add_user, 	addUser_onCardSwipe},
	{RETURN_TO_LAST_STATE_EV, 	add_user, 	addUser_onReturn},
	{ID_OK_EV,					add_user,	addUser_onIdOk},
	{ID_FAIL_EV,				add_user,	addUser_onIdFail},
	{CARD_FAIL_EV,				add_user, 	addUser_onCardFail},
	{ADD_USER_FINISHED_EV, 		menu,		menu_initState},// ver si esta bien la rutina de accion
	{TIMEOUT_EV,				welcome, 	showWelcomeAnimation},
	{FIN_TABLA, 				add_user, 	do_nothing}
};

/*** Config device ***/
STATE cfg_device[] = 
{
	{PRESS_EV,						cfg_device, 	configDev_selectOption},
	{ENCODER_RIGHT_EV, 				cfg_device, 	configDev_nextOption}, 
    {ENCODER_LEFT_EV,				cfg_device, 	configDev_previousOption}, 
	{CONFIG_DEVICE_FINISHED_EV,		menu, 			menu_initState}, // ver si esta bien la rutina de accion
	{TIMEOUT_EV, 					welcome, 		showWelcomeAnimation},
	{FIN_TABLA, 					cfg_device,		do_nothing}
};

/*Delete user*/
STATE delete_user[] = 
{
	{PRESS_EV,						delete_user, 	deleteUser_acceptNumber},
	{ENCODER_RIGHT_EV, 				delete_user, 	deleteUser_increaseCurrent}, 
    {ENCODER_LEFT_EV,				delete_user, 	deleteUser_decreaseCurrent}, 
	{LKP_EV, 						delete_user, 	deleteUser_confirm},
	{CARD_SWIPE_EV, 				delete_user, 	deleteUser_checkCardID},
	{RETURN_TO_LAST_STATE_EV, 		menu,			menu_initState},
	{ID_OK_EV, 						menu, 			menu_initState},
	{TIMEOUT_EV, 					welcome, 		showWelcomeAnimation},
	{FIN_TABLA, 					delete_user, 	do_nothing}
};



/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

//========interfaz=================
STATE *FSM_GetInitState(void)
{
	 return(welcome);
}


///=========Rutinas de accion===============
void FSM_StartInitState()
{
	showWelcomeAnimation();
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
/*Dummy function*/
static void do_nothing(void)
{
	//take some time off, relax, and KEEP WORKING ////you lazy ass ALU
}
