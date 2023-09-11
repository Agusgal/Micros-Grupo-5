/***************************************************************************/ /**
  @file     config_device.c
  @brief    config device state functions
            The main function of the state is to change the brightness of the display.
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdbool.h>
#include "config_device_state.h"
#include "queue.h"
#include "seven_seg_display.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define OPTIONS_ARRAY_SIZE 3
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef enum
{
    MAX_BRIGHT,
    MID_BRIGHT,
    LOW_BRIGHT,
} options_t;
/*******************************************************************************
 * GLOBAL VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static uint8_t currentOptionIndex = 0;  //Stores the index of the current brightness option.

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
/**
 * @brief Sets the current option of the brightness in the display.
 */
static void setCurrentOption(void);
/*******************************************************************************
 * FUNCTIONS WITH GLOBAL SCOPE
 ******************************************************************************/
void initConfigDevice(void)
{
    currentOptionIndex = 0;
    
    SevenSegDisplay_EraseScreen();
    SevenSegDisplay_SetPos(0);
    SevenSegDisplay_ChangeCharacter(0 ,8);
    SevenSegDisplay_ChangeCharacter(1,8);
    SevenSegDisplay_ChangeCharacter(2,8);
    SevenSegDisplay_ChangeCharacter(3,8);
    
    setCurrentOption();
}

void configDev_nextOption(void)
{
        if (currentOptionIndex == OPTIONS_ARRAY_SIZE - 1)   //Last option + Next -> First option.
            currentOptionIndex = 0;
        else
            currentOptionIndex++;
        setCurrentOption();
}

void configDev_previousOption(void)
{
    if (currentOptionIndex == 0)    //First option + Previous -> Last option.
        currentOptionIndex = OPTIONS_ARRAY_SIZE - 1;
    else
        currentOptionIndex--;
    setCurrentOption();
}

void configDev_selectOption(void)
{
    SevenSegDisplay_EraseScreen();
    emitEvent(CONFIG_DEVICE_FINISHED_EV);
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void setCurrentOption(void)
{
    switch (currentOptionIndex)
    {
        case MAX_BRIGHT:
            SevenSegDisplay_SetBright(MAX);
            break;
        case MID_BRIGHT:
            SevenSegDisplay_SetBright(MID);
            break;
        case LOW_BRIGHT:
            SevenSegDisplay_SetBright(MIN);
            break;
    }
}
