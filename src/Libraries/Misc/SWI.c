/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Misc.SWI.c
    Author:  Copyright © 1992 Jason Williams
    Version: 1.00 (22 Mar 1992)
    Purpose: SWI call veneer function
*/

#include <stdarg.h>

#include "kernel.h"

#include "DeskLib:Core.h"
#include "DeskLib:SWI.h"



os_error *SWI(int numregsin, int numregsout, int swicode, ...)
{
  va_list          ap;
  _kernel_swi_regs regsin, regsout;
  _kernel_oserror  *error;
  register int     loop;
  int              *temp;

  va_start(ap, swicode);

  for (loop = 0; loop < numregsin; loop++)
    regsin.r[loop] = va_arg(ap, int);

  error = _kernel_swi(swicode, &regsin, &regsout);

  for (loop = 0; loop < numregsout; loop++)
  {
    temp = va_arg(ap, int *);
    if (temp != NULL)
      *temp = regsout.r[loop];
  }

  va_end(ap);
  return((os_error *) error);
}
