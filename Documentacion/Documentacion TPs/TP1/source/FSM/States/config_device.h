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
 * @brief Function to be called when the configuration ends. It emits a ADMIN_CONFIG_DEVICE_FINISHED_EV event to the event queue.
 */
void finish_configuration(void);

#endif /* CONFIG_DEVICE_STATE_H*/
