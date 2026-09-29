/*
 * This file is part of DeskLib, the C library for RISC OS.
 * Please see accompanying documentation for terms of use.
 *
 *       http://www.riscos.info/index.php/DeskLib
 *
 *
 * Module:  Environment
 * File:    Modules.c
 * Author:  Adam Richardson
 * Purpose: Check for presence of module, list all modules.
 *
 * Version History
 * 28/08/2007: Creation
 *
 */

#include "DeskLib:Environment.h"
#include "DeskLib:SWI.h"
#include "DeskLib:Error.h"

BOOL Environment_ModuleIsActive(const char *modulename)
{
  BOOL returnvalue = FALSE;

  returnvalue = !(Error_CheckSilent(SWI(2, 0, SWI_OS_Module | XOS_Bit, 18, modulename)));

  return returnvalue;
}
