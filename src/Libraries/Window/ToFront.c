/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Window.ToFront.c
    Author:  Copyright © 1992 Jason Williams
    Version: 1.00 (19 Mar 1992)
    Purpose: High-level window management functions: Bring window to front
*/


#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Window.h"

extern void Window_BringToFront(window_handle window)
/* Pulls the window to the front of the window stack */
{
  window_state wstate;

  Wimp_GetWindowState(window, &wstate);
  wstate.openblock.behind = -1;
  Wimp_OpenWindow(&wstate.openblock);
}
