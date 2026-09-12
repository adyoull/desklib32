/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Menu.ReviseWdth.c
    Author:  Copyright © 1994 Lenny
    Version: 0.02 (20 Nov 1994)
    Purpose: Revises a menu's width, according to the longest menu item.
    History: 0.01 (20 Nov 94) : Added 'Menu_ReviseWidth()'
    Mods:    22 Jul 1995 - JPS - Set maxwidth to 0 initially

*/


#include "DeskLib:Wimp.h"
#include "DeskLib:Menu.h"

#include <string.h>


extern void Menu_ReviseWidth(menu_ptr menu)
/*
 *  Revises a menu's width, by scanning through all the items.
 *  The title width is also taken into consideration.
 *  It is assumed that there is AT LEAST ONE item in the menu.
 *
 *  Use it after using Menu_SetText(), or when updating the
 *  contents of an indirected text menu item, ie when having
 *  used Menu_MakeIndirected().
 */
{
  int                   width, maxwidth=0;
  BOOL                  last = FALSE;
  menu_item   *item = (menu_item *) (((int) menu) + sizeof(menu_block));

  do {
    if (item->iconflags.data.indirected) {
      width = strlen(item->icondata.indirecttext.buffer);
    }
    else {
      width = strlen(item->icondata.text);
      if (width > 12)  width = 12;  /* It's a full length non-terminated entry */
    }
    if (width > maxwidth)  maxwidth = width;
    if (item->menuflags.data.last)  last = TRUE;
    item++;
  } while (last == FALSE);
  maxwidth += 1;

  width = strlen(menu->title);
  if (width > 12)  width = 12;  /* It's a full length non-terminated title */
  if (width > maxwidth)  maxwidth = width;

  menu->width = maxwidth * 16;

}

