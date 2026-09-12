/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Window.DeleteOrig.c
    Author:  Copyright © 1995 Sergio Monesi (original code from
                              Window.Delete.c by Jason Williams)
    Version: 1.00 (15 Feb 1995)
    Purpose: High-level window management functions: Delete a window
             created with Window_CreateOrig
*/


#include "DeskLib:LinkList.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Template.h"
#include "DeskLib:Event.h"
#include "DeskLib:EventMsg.h"
#include "DeskLib:Window.h"
#include "DeskLib:Screen.h"
#include "DeskLib:Error.h"
#include "DeskLib:Window.h"

#include "WindowDefs.h"

#include <stdlib.h>
#include <string.h>


extern linklist_header window_listanchor;



extern void Window_DeleteOrig(window_handle window)
{
  windowrec       *ptr;

  Event_ReleaseWindow(window);       /* Release all handlers for this window */
  EventMsg_ReleaseWindow(window);

  Wimp_CloseWindow(window);                                  /* say bye bye! */
  Wimp_DeleteWindow(window);

  ptr = (windowrec *) window_listanchor.next;
  while (ptr != NULL)
  {
    if (ptr->window == window)
      break;
    ptr = (windowrec *) ptr->header.next;
  }

  if (ptr == NULL)
    return;              /* Window not created with Window_Show(). Bad user! */

  LinkList_Unlink(&window_listanchor, &(ptr->header));

  free(ptr);                         /* Free up the window's list entry      */
}
