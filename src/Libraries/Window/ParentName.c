/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Window.ParentName.c
    Author:  Copyright © 1992 Jason Williams
    Version: 1.00 (19 Mar 1992)
    Purpose: High-level window management functions: Return template window
             was created from (if known)
*/


#include <string.h>

#include "DeskLib:LinkList.h"
#include "DeskLib:Window.h"

#include "WindowDefs.h"

extern linklist_header window_listanchor;



extern void Window_ParentName(window_handle window, char *windowname)
{
  windowrec     *record;

  if (window < 0)
  {
    strcpy(windowname, "iconbar");
    return;
  }

  windowname[0] = '\0';     /* in case window not found, ensure valid output */

  record = (windowrec *) window_listanchor.next;
  while (record != NULL)
  {
    if (window == record->window)
      break;
    record = (windowrec *) record->header.next;
  }

  if (record != NULL)
  {
    strncpy(windowname, record->templatename, wimp_MAXNAME);
    windowname[wimp_MAXNAME - 1] = 0;
  }
}
