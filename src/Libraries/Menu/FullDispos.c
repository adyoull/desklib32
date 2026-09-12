/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Menu.FullDispos.c
    Author:  Copyright © 1994 Tony Houghton
    Version: 1.01 (15 Nov 1994)
    Purpose: Dispose of a menu and its indirected data.
*/

#include <stdlib.h>

#include "DeskLib:Menu.h"

#define ENTRY(menu) ((menu_item *) ((int) (menu) + sizeof(menu_block)))


void Menu_FullDispose(menu_ptr menu)
{
  int index;
  menu_item *items = ENTRY(menu);

  /* Free title data if necessary */
  if (items[0].menuflags.data.indtitle)
    free (((icon_data *)(void *)menu->title)->indirecttext.buffer);
    /* Menu_New makes title validation -1 */

  /* Process entries */
  for (index = 0;;index++)
  {
    if (items[index].iconflags.data.indirected)
    {
      free(items[index].icondata.indirecttext.buffer);

      if ((int) items[index].icondata.indirecttext.validstring > 0)
        free(items[index].icondata.indirecttext.validstring);
    }

    if (items[index].menuflags.data.last)
      break;
  }
}
