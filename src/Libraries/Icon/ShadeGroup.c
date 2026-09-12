/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Icon.ShadeGroup.c
    Author:  Copyright © 1994 Tim Browse
    Version: 1.00 (05 Mar 1994)
    Purpose: Shade/unshade a group of icons, specified by an array of
             icon handles (-1 terminated).
*/

#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"


extern void Icon_ShadeGroup(window_handle window,
                            icon_handle icons[],
                            BOOL fade_flag_value)
{
  int i = 0;

  while (icons[i] != -1)
  {
    if (fade_flag_value)
      Icon_Shade(window, icons[i]);
    else
      Icon_Unshade(window, icons[i]);

    i++;
  }
}
