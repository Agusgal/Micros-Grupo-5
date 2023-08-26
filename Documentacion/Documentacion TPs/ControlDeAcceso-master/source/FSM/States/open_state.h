/***************************************************************************/ /**
  @file     open_state.h
  @brief    open state header.
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

#ifndef OPEN_STATE_H_
#define OPEN_STATE_H_

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Opens the door.
 */
void openDoor(void);

/**
 * @brief Function to be called by a timer indicating that the door needs to be closed.
 */
void accessTimeOut(void);

#endif /* OPEN_STATE_H_*/
