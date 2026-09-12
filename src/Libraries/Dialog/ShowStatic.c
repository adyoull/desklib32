/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Dialog.ShowStatic.c
    Author:  Copyright © 1993 Tim Browse and Jason Williams
    Version: 1.00 (10 Jul 1993)
    Purpose: Very high level window (dialogue) handling -
             Showing permanent (static) dialogue windows
*/


#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"

#include "DeskLib:Dialog.h"
#include "DeskLib:Window.h"


extern void Dialog_ShowStatic(dialog dbox, window_openpos openpos)
/*  Shows a dialogue box at the position requested (see Window.h for
 *  window_openpos values). The dialoge box will stay until expressly
 *  closed by the user or your program.
 */
{
  dbox->state.isstatic = dbox->state.stillopen = TRUE;
  Window_Show(dbox->window, openpos);
}
