/***************************************************************************/ /**
  @file     fsm.c
  @brief    FSM functions.
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdio.h>
#include "fsm.h"

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
STATE *fsm(STATE *p_tabla_estado, event evento_actual)
{
   while (p_tabla_estado->evento != evento_actual //Recorre las tablas de estado
          && p_tabla_estado->evento != FIN_TABLA)
      ++p_tabla_estado;
   (*p_tabla_estado->p_rut_accion)();               /*rutina de accion corresondiente*/
   p_tabla_estado = p_tabla_estado->proximo_estado; /*siguiente estado*/

   return (p_tabla_estado);
}

