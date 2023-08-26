/***************************************************************************/ /**
  @file     config_device_state.h
  @brief    config device state header.
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

#ifndef CONFIG_DEVICE_STATE_H
#define CONFIG_DEVICE_STATE_H

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Initialization of the state.
 */
void initConfigDevice(void);
/**
 * @brief Changes the current position of the ID array to the next character following CHAR ORDER.
 */
void configDev_nextOption(void);
/**
 * @brief Shows the next welcome option.
 */
void configDev_previousOption(void);

/**
 * @brief Selects the current option and changes the bright of the display. It emits a ADMIN_CONFIG_DEVICE_FINISHED_EV event to the event queue.
 */
void configDev_selectOption(void);

#endif /* CONFIG_DEVICE_STATE_H*/
