/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Window.CreateOrig.c
    Author:  Copyright © 1995 Sergio Monesi (original code from
                              Window.Create.c by Jason Williams)
    Version: 1.00 (15 Feb 1995)
    Purpose: High-level window management functions: Create a window
             without making a copy of the template (ie. use the original
             template)
*/


#include "DeskLib:LinkList.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Template.h"
#include "DeskLib:Event.h"
#include "DeskLib:Window.h"
#include "DeskLib:Screen.h"
#include "DeskLib:Error.h"

#include "WindowDefs.h"

#include <stdlib.h>
#include "string.h"


#define ERRBASE 1
#define ERR1 ERRBASE+0
#define ERRMESS1 "Insufficient memory to open window"


extern linklist_header window_listanchor;


extern window_handle Window_CreateOrig(const char *windowname)
{
  windowrec     *record;
  window_block  *windowptr;
  window_handle window;

  windowptr = Template_Find(windowname);
  if (windowptr == NULL)
    return(0);

  Wimp_CreateWindow(windowptr, &window);

  record = (windowrec *) malloc(sizeof(windowrec));
  if (record == NULL)  Error_ReportFatalInternal(ERR1, ERRMESS1);

  LinkList_AddToHead(&window_listanchor, &(record->header));

  strncpy(record->templatename, windowname, wimp_MAXNAME);
  record->templatename[wimp_MAXNAME] = 0;       /* Record of name for help   */

  record->window = window;                      /* Remember window handle    */
  record->memory = windowptr;                   /* remember to dealloc later */

  return(window);
}

