/*
 * This file is part of DeskLib, the C library for RISC OS.
 * Please see accompanying documentation for terms of use.
 *
 *       http://www.riscos.info/index.php/DeskLib
 *
 *
 * Module:  Debug
 * File:    Reporter.c
 * Author:  Adam Richardson
 * Purpose: Send debug output to Martin Avison's "Reporter"
 *
 * Version History
 * 18/08/2007: Creation
 *
 */

#include "DeskLib:Debug.h"
#include "DeskLib:SWI.h"
#include "DeskLib:Environment.h"

#include "DebugDefs.h"

BOOL Debug_InitialiseReporter(void)
{
  if (Environment_TaskIsActive("Reporter"))
    return FALSE;
  else
    /* Reporter is not active, so return fail code */
    return TRUE;
}

void Debug_PrintReporter(const char *text)
{
  SWI(1 ,0 , SWI_Reporter_Text0 | XOS_Bit, text);
}


