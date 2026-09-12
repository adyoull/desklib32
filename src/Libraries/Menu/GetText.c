/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Menu.GetText.c
    Author:  Copyright © 1993 Jason Howat
    Version: 1.00 (13 Dec 1993)
    Purpose: Gets the address of the text in a menu item
*/

#include <string.h>
#include <stdlib.h>

#include "DeskLib:Wimp.h"
#include "DeskLib:Menu.h"


char *Menu_GetText(menu_ptr menu, int entry)
{
  char      *text;
  menu_item *item = (menu_item*) (((int) menu) + sizeof(menu_block));

  item = &item[entry];

  if (item->iconflags.data.indirected)
    text = item->icondata.indirecttext.buffer;
  else
    text = item->icondata.text;

  return text;
}
