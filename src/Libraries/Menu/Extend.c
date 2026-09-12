/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Menu.Extend.c
    Author:  Copyright © 1993 Shaun Blackmore and Jason Williams
    Version: 1.00 (30 Apr 1993)
    Purpose: Extends a menu by adding entries to the bottom of it
*/

#include <stdlib.h>

#include "DeskLib:Error.h"
#include "DeskLib:Screen.h"
#include "DeskLib:Wimp.h"
#include "DeskLib:Menu.h"

#include "MenuDefs.h"


extern menu_ptr Menu_Extend(menu_ptr menu, const char *description)
{
  menu_ptr  newmenu;
  unsigned int   maxwidth = 0;
  unsigned int   newitems = 0, lastitem = 0;
  menu_item *item = (menu_item *) ((int) menu + sizeof(menu_block));

  Menu__CountItems(description, &newitems, &maxwidth);

  maxwidth *= 8 * screen_delta.x;
  if (menu->width < maxwidth)
    menu->width = maxwidth;

  while (!item[lastitem].menuflags.data.last)         /* Find the last item */
    lastitem++;

  newmenu = realloc((void *) menu, sizeof(menu_block) +
                             (sizeof(menu_item) * (lastitem + 1 + newitems)));
  if (newmenu == NULL)
    return((menu_ptr) Error_OutOfMemory(FALSE, "menus"));

  item = (menu_item *) ((int) newmenu + sizeof(menu_block));
  item[lastitem].menuflags.data.last = FALSE;

  if (!Menu__Create(&item[lastitem + 1], description, newitems))
    return(menu);

  item[lastitem + newitems].menuflags.data.last = TRUE;

  return(newmenu);
}
