/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    GFX.ReadPoint.c
    Author:  Copyright © 2003 Antony Sidwell
    Version: 1.00 (24 Jun 2003)
    Purpose: Read the colour & tint of a point on the screen
             This should really be done in ASM eventually
*/

#include "DeskLib:GFX.h"
#include "DeskLib:SWI.h"    

#ifndef SWI_OS_ReadPoint
#define SWI_OS_ReadPoint 0x32
#endif

extern BOOL GFX_ReadPoint(int x, int y, int *colour, int *tint)
{
  int onscreen = 0;

  SWI(2, 5, SWI_OS_ReadPoint, x, y, NULL, NULL, colour, tint, &onscreen);

  return (onscreen == 0) ? TRUE : FALSE;
}
