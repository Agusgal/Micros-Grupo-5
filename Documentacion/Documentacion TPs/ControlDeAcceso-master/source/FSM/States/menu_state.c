/***************************************************************************/ /**
  @file     menu_state.c
  @brief    menu_state
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <States/menu_state.h>
#include "queue.h"
#include "seven_seg_display.h"
#include "Timer.h"
#include "data_base.h"
#include "Led.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define ADMIN_OPTIONS_ARRAY_SIZE 6
#define USER_OPTIONS_ARRAY_SIZE 3
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef enum
{
    OPEN,
    CONFIG_PIN,
    LOG_OUT,
    CONFIG_DEVICE,
    ADD_USER,
    DEL_USER
} options_t;
/*******************************************************************************
 * GLOBAL VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static uint8_t currentOptionIndex = 0;
static bool showingTitle;
static int titleTimerID = -1;
static bool admin = false;
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define TITLE_TIME  2000

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
/**
 * @brief Show the title of the state in the display. If the user interacts with the system, the title will stop showing and the input will start.
 */
static void showTitle(void);
/**
 * @brief Stops showing the title of the state in the display. The input starts.
 */
static void stopShowingTitle(void);

/**
 * @brief Stops showing the title of the state in the display due to a user's interaction. The input starts.
 */
static void userInteractionStopsTitle(void);

/**
 * @brief Shows the current option of the welcome in the display.
 */
static void setCurrentOption(void);
/*******************************************************************************
 * FUNCTIONS WITH GLOBAL SCOPE
 ******************************************************************************/

void menu_initState(void)
{
    showingTitle = false;
    admin = IsAdmin();
    currentOptionIndex = 0;
    showTitle();
}

void menu_nextOption(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else
    {
        uint8_t max = admin ? ADMIN_OPTIONS_ARRAY_SIZE: USER_OPTIONS_ARRAY_SIZE;
        if (currentOptionIndex == max - 1)
            currentOptionIndex = 0;
        else
            currentOptionIndex++;
        setCurrentOption();
    }
    
}

void menu_previousOption(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else
    {
        uint8_t max = admin ? ADMIN_OPTIONS_ARRAY_SIZE: USER_OPTIONS_ARRAY_SIZE;
        if (currentOptionIndex == 0)
            currentOptionIndex = max - 1;
        else
            currentOptionIndex--;
        setCurrentOption();
    } 
}

void menu_selectOption(void)
{
    if (showingTitle)
        userInteractionStopsTitle();
    else
    {
        SevenSegDisplay_EraseScreen();
        SevenSegDisplay_SetPos(0);
        switch (currentOptionIndex)
        {
            case OPEN:
                emitEvent(OPEN_SELECTED_EV);
                break;
            case CONFIG_DEVICE:
                emitEvent(CONFIG_DEVICE_SELECTED_EV);
                break;
            case ADD_USER:
                emitEvent(ADD_USER_SELECTED_EV);
                break;
            case DEL_USER:
                emitEvent(DELETE_USER_SELECTED_EV);
                break;
            case CONFIG_PIN:
                emitEvent(CONFIG_PIN_SELECTED_EV);
                break;
            case LOG_OUT:
                emitEvent(TIMEOUT_EV);
                break;
        }
    }
    
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void showTitle(void)
{
    SevenSegDisplay_EraseScreen();
    SevenSegDisplay_CursorOff();
    SevenSegDisplay_SetPos(0);
    if (admin)
    {
        SevenSegDisplay_WriteBuffer("BOSS", 4, 0);    
    }
    else
    {
        SevenSegDisplay_WriteBuffer("USER", 4, 0);
    }
    Led_Off(LED_BLUE);
    Led_InfiniteBlink(LED_GREEN, NORMAL);
    titleTimerID = Timer_AddCallback(&stopShowingTitle,TITLE_TIME, true );
}

static void setCurrentOption(void)
{
    SevenSegDisplay_EraseScreen();
    SevenSegDisplay_SetPos(0);
    switch (currentOptionIndex)
    {
    case OPEN:
        SevenSegDisplay_WriteBuffer("OPEN", 4, 0);
        break;
    case CONFIG_DEVICE:
        SevenSegDisplay_WriteBufferAndMove("BRIGHT", 6, 0, BOUNCE);
        break;
    case ADD_USER:
        SevenSegDisplay_WriteBufferAndMove("ADD USER", 8, 0, BOUNCE);
        break;
    case DEL_USER:
        SevenSegDisplay_WriteBufferAndMove("DELETE USER", 11, 0, BOUNCE);
        break;
    case CONFIG_PIN:
        SevenSegDisplay_WriteBufferAndMove("CHANGE PIN", 10, 0, BOUNCE);
        break;
    case LOG_OUT:
        SevenSegDisplay_WriteBufferAndMove("LOG OUT", 7, 0, BOUNCE);
        break;
    }
}

static void stopShowingTitle(void)
{
    SevenSegDisplay_EraseScreen();
    showingTitle = false;
    setCurrentOption();
    //!ojo agregado
    Led_StopInfiniteBlink(LED_GREEN);
    Led_On(LED_BLUE);
}

static void userInteractionStopsTitle(void)
{
    Timer_Delete(titleTimerID);
    titleTimerID = -1;
    stopShowingTitle();
}
