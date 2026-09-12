/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Menu.MakeWrite.c
    Author:  Copyright © 1993 Shaun Blackmore and Jason Williams
    Version: 1.00 (30 Apr 1993)
    Purpose: Makes a menu item writable
*/


#include "DeskLib:Core.h"
#include "DeskLib:Wimp.h"
#include "DeskLib:Menu.h"


void Menu_MakeWritable(menu_ptr menu, int entry,
                       char *buffer, int size, char *valid)
{
  menu_item *item = (menu_item *) (((int) menu) + sizeof(menu_block));

  item = &item[entry];
  item->menuflags.data.writable   = TRUE;
  item->iconflags.data.indirected = TRUE;

  item->icondata.indirecttext.buffer      = buffer;
  item->icondata.indirecttext.bufflen     = size;
  item->icondata.indirecttext.validstring = valid;
}
