/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Menu.PopUp.c
    Author:  Copyright © 1994 Tony Houghton
    Version: 1.00 (25 Sep 1994)
    Purpose: Pop up a menu to the right of a button icon.
*/

#include "DeskLib:Menu.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Coord.h"


void Menu_PopUp(menu_ptr menu,window_handle window,icon_handle icon)
{
  window_state wstate;
  icon_block istate;

  /* Find position of icon on screen */
  Wimp_GetWindowState(window,&wstate);
  Wimp_GetIconState(window,icon,&istate);
  Coord_PointToScreen(&istate.workarearect.max,
                      (convert_block *) &wstate.openblock.screenrect);

  /* Open menu here (Menu_Show subtracts 64 from X xoord) */
  Menu_Show(menu,istate.workarearect.max.x+64,istate.workarearect.max.y);
}
