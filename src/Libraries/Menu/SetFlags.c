/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Menu.SetFlags.c
    Author:  Copyright © 1993 Shaun Blackmore and Jason Williams
    Version: 1.00 (30 Apr 1993)
    Purpose: Sets ticked/shaded flags for a menu item
*/

#include "DeskLib:Wimp.h"
#include "DeskLib:Menu.h"


void Menu_SetFlags(menu_ptr menu, int entry, int ticked, int shaded)
{
  menu_item *item = (menu_item *) (((int) menu) + sizeof(menu_block));

  item = &item[entry];
  item->menuflags.data.ticked = ticked;
  item->iconflags.data.shaded = shaded;
}
