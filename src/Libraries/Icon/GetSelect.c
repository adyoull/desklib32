/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Icon.c.GetSelect
    Author:  Copyright © 1993 Philip Colmer
    Version: 1.00 (14 Jul 1993)
    Purpose: Returns the selection status of an icon
*/

#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"


extern BOOL Icon_GetSelect(window_handle window, icon_handle icon)
/* This is a very inefficient function to use if you want to use the
 * icon state data for anything else, but if you only want to know
 * if the icon is selected, it's much more convenient
 */
{
  icon_block istate;

  Wimp_GetIconState(window, icon, &istate);
  return(istate.flags.data.selected);
}
