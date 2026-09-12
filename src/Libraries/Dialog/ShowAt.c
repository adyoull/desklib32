/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Dialog.ShowAt.c
    Author:  Copyright © 1994 Peter Gaunt
    Version: 1.00 (12 Mar 1994)
    Purpose: Very high level window (dialogue) handling -
             Showing non-permanent (menu) dialogues
*/


#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"

#include "DeskLib:Dialog.h"


/* Similar to Dialog_Show but opens dialogue at x/y */
extern void Dialog_ShowAt( dialog dbox, int x, int y )
{
  dbox->state.isstatic = FALSE;
  Wimp_CreateMenu( (menu_block *) dbox->window, x, y);
  dbox->state.stillopen = TRUE;
}
