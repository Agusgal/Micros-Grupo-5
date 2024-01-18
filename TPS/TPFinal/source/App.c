/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>


#include <FSM_1/FSM.h>

#include "Drivers/HAL/Encoder.h"

#include "fsl_common.h"

#include "gpio.h"
#include "board.h"

#include "SysTick.h"
#include "Timer.h"


#include "AudioPlayer.h"

#include "EventQueue/queue.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

int testFunc(void);



/**
 * @brief captures events generated within the drivers and inside states and fills the eventqueue. Gets called continously.
 * @return nothing.
 */
void fill_queue(void);

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


//this variable saves the current state of the FSM
static state *current_state;


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	PowerMode_Init();

	SysTick_Init();
	Timer_Init();

	//Memory Init
	//OLED Init
	//Matrix Init
	md_Init();  //Agrego implementacion de la documentacion para poder inicializar el audio player.

	//MP3 decoder Init

	//Audio PLayer Init, para poder inicializarlo debe estar inicializado el DMA, sino tira error turbio.
	AudioPlayer_Init();

	//Vumeter/FFT Init
	//Equalizer Init

	//Daytime Init

	//Init Queue
	queue_Init();

	//Init Encoder
	Encoder_Init();

	//Init fsm
	current_state = get_initial_state();
	start_fsm();
}



/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	testFunc();

	fill_queue();

	Event_Type event = pull_Queue_Element();

	if (event != NONE_EV)
	{
		current_state = fsm_dispatcher(current_state, event);
	}

}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/**
 * @brief captures events generated within the drivers and inside states and fills the eventqueue. Gets called continously.
 * @return nothing.
 */
void fill_queue(void)
{

	//check for encoder events


	//Check for Encoder press events


	//Check for button events


	//Check for OLEd events


	//Check for AudioPLayer Events
	if (AudioPlayer_IsBackBufferFree())
	{
		push_Queue_Element(FILL_BUFFER_EV);
	}


	//Check for memory events
}



/*
 *---------------------PROGRAMA DE PRUEBA PANTALLA SSD1306 - 128*64 ---------------------
 *
 * */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ACCEL_I2C_CLK_SRC     I2C0_CLK_SRC
#define ACCEL_I2C_CLK_FREQ    CLOCK_GetFreq(I2C0_CLK_SRC)
#define I2C_BAUDRATE          400000
//#define I2C_RELEASE_SDA_PORT  PORTE
//#define I2C_RELEASE_SCL_PORT  PORTE
//#define I2C_RELEASE_SDA_GPIO  GPIOE
//#define I2C_RELEASE_SDA_PIN   25U
//#define I2C_RELEASE_SCL_GPIO  GPIOE
//#define I2C_RELEASE_SCL_PIN   24U
//#define I2C_RELEASE_BUS_COUNT 100U


#include <stdio.h>
#include "board.h"
//#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_i2c.h"
#include "fsl_port.h"
//#include "fsl_swm.h"
#include "OLEDdisplay.h"


static void I2C_InitModule(void)
{
    i2c_master_config_t masterConfig;
    uint32_t sourceClock = 0;

    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = I2C_BAUDRATE;
    sourceClock               = ACCEL_I2C_CLK_FREQ;
    I2C_MasterInit(I2C0, &masterConfig, sourceClock);
}

void BOARD_InitPins(void)
{
	/* Port E Clock Gate Control: Clock enabled */
	CLOCK_EnableClock(kCLOCK_PortE);
	/* PORTE24 (pin 31) is configured as I2C0_SCL */
	PORT_SetPinMux(PORTE, 24U, kPORT_MuxAlt5);

	/* PORTE25 (pin 32) is configured as I2C0_SDA */
	PORT_SetPinMux(PORTE, 25U, kPORT_MuxAlt5);
}

void BOARD_I2C_ConfigurePins(void)
{
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    const port_pin_config_t porte24_pin31_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is enabled */
                                                    kPORT_OpenDrainEnable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as I2C0_SCL */
                                                    kPORT_MuxAlt5,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTE24 (pin 31) is configured as I2C0_SCL */
    PORT_SetPinConfig(PORTE, 24U, &porte24_pin31_config);

    const port_pin_config_t porte25_pin32_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is enabled */
                                                    kPORT_OpenDrainEnable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as I2C0_SDA */
                                                    kPORT_MuxAlt5,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTE25 (pin 32) is configured as I2C0_SDA */
    PORT_SetPinConfig(PORTE, 25U, &porte25_pin32_config);
}


int testFunc(void)
{
	// I2C OnBoard Pinout
	//#define I2C0_SCL_PIN				24
	//#define I2C0_SDA_PIN				25

	//CLOCK_Select(kI2C1_Clk_From_MainClk);
	//CLOCK_EnableClock(kCLOCK_Swm);
	//SWM_SetMovablePinSelect(SWM0, kSWM_I2C1_SDA, kSWM_PortPin_P0_28);  // SDA to PIO0_28
	//SWM_SetMovablePinSelect(SWM0, kSWM_I2C1_SCL, kSWM_PortPin_P0_27);  // SCL to PIO0_27
	//CLOCK_DisableClock(kCLOCK_Swm);

	//PORTE->PCR[I2C0_SDA_PIN] = 0x0; //Clear all bits
	//PORTE->PCR[I2C0_SDA_PIN]|= PORT_PCR_IRQC(PORT_eDisabled); //Disable interrupts
	//PORTE->PCR[I2C0_SDA_PIN]|= PORT_PCR_MUX(PORT_mAlt5); 	 //Set MUX to I2C0
	//PORTE->PCR[I2C0_SDA_PIN]|= PORT_PCR_ODE_MASK;

	//PORTE->PCR[I2C0_SCL_PIN] = 0x0; //Clear all bits
	//PORTE->PCR[I2C0_SCL_PIN]|= PORT_PCR_IRQC(PORT_eDisabled); //Disable interrupts
	//PORTE->PCR[I2C0_SCL_PIN]|= PORT_PCR_MUX(PORT_mAlt5); 	 //Set MUX to I2C0
	//PORTE->PCR[I2C0_SCL_PIN]|= PORT_PCR_ODE_MASK;


	//   Variables
	//uint32_t baudRate = 400000;
	//uint32_t frecuency = 1000000;

	//i2c_master_config_t  i2config; // config. variable. i2c
	//I2C_MasterGetDefaultConfig(&i2config);                             // get the default config of i2c
	//i2config.baudRate_Bps = baudRate;                                  //adjusting the baudrate to the OLED oscillator
	//I2C_MasterInit(I2C1, &i2config, frecuency);                        //initialization



	BOARD_InitPins();
	BOARD_InitBootClocks();

	BOARD_I2C_ConfigurePins();

	I2C_InitModule();






    // Initialize the SSD1306 display
    OLED_Init();
    OLED_Refresh();
    OLED_Clear();

    // Print welcome message
    //OLED_Copy_Image(&logo_nxp[0], sizeof(logo_nxp));
    //OLED_Refresh();

    OLED_Set_Text(10, 52, kOLED_Pixel_Set, "INDIE", 2);
    OLED_Refresh();


    while(1) {

    }
    return 0 ;
}



