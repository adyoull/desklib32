/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Window.CreateShow.c
    Author:  Copyright © 1992 Jason Williams
    Version: 1.00 (19 Mar 1992)
    Purpose: High-level window management functions: Show/hide a window
*/

#include "DeskLib:Window.h"

extern window_handle Window_CreateAndShow(const char    *windowname,
                                          int            maxtitlesize,
                                          window_openpos openpos)
{
  window_handle temp;

  temp = Window_Create(windowname, maxtitlesize);
  Window_Show(temp, openpos);

  return(temp);
}
