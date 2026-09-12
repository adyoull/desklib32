/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Menu.GetFlags.c
    Author:  Copyright © 1993 Jason Howat
    Version: 1.00 (13 Dec 1993)
    Purpose: Gets ticked/shaded flags for a menu item
*/

#include "DeskLib:Wimp.h"
#include "DeskLib:Menu.h"

void Menu_GetFlags(menu_ptr menu, int entry, int *ticked, int *shaded)
{
  menu_item *item = (menu_item *) (((int) menu) + sizeof(menu_block));

  item = &item[entry];
  if(ticked)
    *ticked = item->menuflags.data.ticked;
  if(shaded)
    *shaded = item->iconflags.data.shaded;
}
