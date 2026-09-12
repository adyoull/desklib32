/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Dialog.Hide.c
    Author:  Copyright © 1993, 1994 Tim Browse and Jason Williams
    Version: 1.00 (02 Mar 1994)
    Purpose: Very high level window (dialogue) handling -
             Showing non-permanent (menu) dialogues
*/


#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"

#include "DeskLib:Dialog.h"
#include "DeskLib:Screen.h"


extern void Dialog_Hide(dialog d)
{
  if (d->state.isstatic)
  {
    /* Just close the window */
    Wimp_CloseWindow(d->window);
  }
  else if (d->state.stillopen)
  {
    /* Window is actually a menu, and it is still open, so close the
       menu, in case the Wimp gets upset.
    */
    Wimp_CreateMenu((menu_block *) -1, 0, 0);
  }
}

