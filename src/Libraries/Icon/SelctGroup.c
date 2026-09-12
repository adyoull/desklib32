/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Icon.SelctGroup.c
    Author:  Copyright © 1994 Tim Browse
    Version: 1.00 (05 Mar 1994)
    Purpose: Select/deselect a group of icons, specified by an array of
             icon handles.
*/

#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"


void Icon_SelectGroup(window_handle window,
      	      	      icon_handle icons[],
      	      	      BOOL select_flag_value)
{
  int i = 0;

  while (icons[i] != -1)
  {
    if (select_flag_value)
      Icon_Select(window, icons[i]);
    else
      Icon_Deselect(window, icons[i]);

    i++;
  }
}
