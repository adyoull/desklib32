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
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Menu.h"


wimp_point menu_currentpos;
menu_ptr   menu_currentopen = NULL;


int Menu_CalcHeight(menu_ptr menu)
{
  int       itemheight, count = 0;
  menu_item *item;

  item = (menu_item *) (((int) menu) + sizeof(menu_block));
  itemheight = menu->height + menu->gap;

  while(TRUE)
  {
    if (item->menuflags.data.dotted)         /* Count height of dotted line  */
      count += 24;

    count += itemheight;                     /* Plus the height of each item */

    if (item->menuflags.data.last)
      break;

    item++;
  }

  return(count);
}


void Menu_Show(menu_ptr menu, int x, int y)
{
  x -= 64;
  if (y < 0)
    y = 96 + Menu_CalcHeight(menu);            /* Calculate iconbar position */

  menu_currentopen  = menu;
  menu_currentpos.x = x;
  menu_currentpos.y = y;

  Wimp_CreateMenu(menu, x, y);
}


void Menu_ShowLast(void)
{
  Wimp_CreateMenu(menu_currentopen, menu_currentpos.x, menu_currentpos.y);
}
