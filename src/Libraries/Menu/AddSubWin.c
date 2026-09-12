/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Menu.AddSubWin.c
    Author:  Copyright © 2002 Antony Sidwell
    Version: 1.00 (28 Sep 2002)
    Purpose: Adds a window to a menu as a "submenu"
*/

#include "DeskLib:Wimp.h"
#include "DeskLib:Menu.h"


void Menu_AddSubWindow(menu_ptr menu, int entry, window_handle window)
{
  menu_item *item = (menu_item*) (((int) menu) + sizeof(menu_block));

  item[entry].submenu.window = window;
}
