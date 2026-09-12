/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Icon.ScreenPos.c
    Author:  Copyright © 1994 Peter Gaunt
    Version: 1.00 (12 Mar 1994)
    Purpose: Get the position of an icon in screen coordinates.
*/

#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Coord.h"
#include "DeskLib:Icon.h"


extern void Icon_ScreenPos(window_handle window,
                           icon_handle   icon,
                           wimp_rect     *rect)
{
  icon_block   istate;
  window_state wstate;

  Wimp_GetWindowState( window, &wstate );
  Wimp_GetIconState( window, icon, &istate );

  Coord_RectToScreen( &istate.workarearect,
                      (convert_block *) &wstate.openblock.screenrect );

  *rect = istate.workarearect;

}
