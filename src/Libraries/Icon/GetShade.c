/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Icon.GetShade.c
    Author:  Copyright © 1993 Jason Howat
    Version: 1.00 (14 Jul 1993)
    Purpose: Retrieve the current 'shading' state of the given icon.
*/

#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"


extern BOOL Icon_GetShade(window_handle window, icon_handle icon)
/* The current state of the given icon's 'shading' is returned. */
{
  icon_block istate;

  Wimp_GetIconState(window, icon, &istate);
  return(istate.flags.data.shaded);
}
