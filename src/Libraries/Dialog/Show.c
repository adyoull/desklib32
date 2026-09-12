/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Dialog.Show.c
    Author:  Copyright © 1993 Tim Browse and Jason Williams
    Version: 1.00 (10 Jul 1993)
    Purpose: Very high level window (dialogue) handling -
             Showing non-permanent (menu) dialogues
*/


#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"

#include "DeskLib:Dialog.h"
#include "DeskLib:Screen.h"


extern void Dialog_Show(dialog dbox)
/* Open the window in the centre of the screen as a menu */
{
  window_state  state;
  wimp_point    win_size, pos;

  dbox->state.isstatic = FALSE;

  /* Get the window size */
  Wimp_GetWindowState(dbox->window, &state);
  win_size.x = state.openblock.screenrect.max.x -
                 state.openblock.screenrect.min.x;
  win_size.y = state.openblock.screenrect.max.y -
                 state.openblock.screenrect.min.y;

  /*  Open it as a Menu so that clicks outside it and hitting escape will
   *  close it. Open it in the centre of the screen
   */
  pos.x = (screen_size.x - win_size.x) / 2;
  pos.y = (screen_size.y + win_size.y) / 2;
  Wimp_CreateMenu((menu_block *) dbox->window, pos.x, pos.y);

  dbox->state.stillopen = TRUE;
}

