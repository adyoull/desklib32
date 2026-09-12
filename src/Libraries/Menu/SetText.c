/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Menu.SetText.c
    Author:  Copyright © 1993 Shaun Blackmore and Jason Williams
    Version: 1.00 (30 Apr 1993)
    Purpose: Changes the text in a menu item
*/

#include <string.h>
#include <stdlib.h>

#include "DeskLib:Wimp.h"
#include "DeskLib:Menu.h"


void Menu_SetText(menu_ptr menu, int entry, const char *text)
{
  int       length, oldlength;
  char      *dest;
  menu_item *item = (menu_item*) (((int) menu) + sizeof(menu_block));

  item = &item[entry];

  /*  If the new text will not fit into the old buffer, then
   *    if it was indirected
   *      Deallocate the old indirected buffer
   *    Allocate a new buffer
   *  NOTE: we don't bother shrinking the buffer if the new text is shorter
   */

  if (item->iconflags.data.indirected)
    oldlength = item->icondata.indirecttext.bufflen;
  else
    oldlength = wimp_MAXNAME;

  if ((length = strlen(text)) >= oldlength)
  {
    if (item->iconflags.data.indirected)
      free(item->icondata.indirecttext.buffer);

    item->icondata.indirecttext.buffer = malloc(length + 1);
    if (item->icondata.indirecttext.buffer != NULL)
    {
      item->iconflags.data.indirected         = TRUE;
      item->icondata.indirecttext.bufflen     = length + 1;
      item->icondata.indirecttext.validstring = (char *) -1;
    }
  }

  /*  Copy the new string into the buffer, truncating it if it is still
   *  too long to fit into the buffer.
   */

  if (item->iconflags.data.indirected)
  {
    length = item->icondata.indirecttext.bufflen - 1;
    dest   = item->icondata.indirecttext.buffer;
  }
  else
  {
    length = wimp_MAXNAME - 1;
    dest   = item->icondata.text;
  }

  strncpy(dest, text, length);    /* Copy the string (truncate if neccesary) */
  dest[length] = 0;               /* And ensure it is zero terminated        */
}
