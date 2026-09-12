/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    KernelSWIs.ReadVarVal.c
    Author:  Copyright © 1994 Tim Browse
    Version: 1.00 (30 May 1994)
    Purpose: Function to read an environment variable's value.
*/

#include "DeskLib:Core.h"
#include "DeskLib:KernelSWIs.h"
#include "DeskLib:SWI.h"


extern BOOL OS_ReadVarVal(const char *varname, char *buf, int bufsize)
{
  int result;

  /* Check for existence of this system variable */
  SWI(3, 3, SWI_OS_ReadVarVal, varname, buf, -1, NULL, NULL, &result);
  if (result < 0)
  {
    /* Variable exists - read it and use text for file description */
    SWI(5, 3, SWI_OS_ReadVarVal,
        varname, buf, bufsize, 0, 0,
        NULL, NULL, &result);

    /* Ensure correct termination */
    buf[result] = 0;

    return TRUE;
  }

  /* Could not find it */
  return FALSE;
}

