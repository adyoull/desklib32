/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Menu.AddSubMenu.c
    Author:  Copyright © 1993 Shaun Blackmore and Jason Williams
    Version: 1.00 (30 Apr 1993)
    Purpose: Adds one menu to another as a submenu
*/

#include "DeskLib:Wimp.h"
#include "DeskLib:Menu.h"


void Menu_AddSubMenu(menu_ptr menu, int entry, menu_ptr submenu)
{
  menu_item *item = (menu_item*) (((int) menu) + sizeof(menu_block));

  item[entry].submenu.menu = submenu;
}
