/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Window.Create.c
    Author:  Copyright © 1992, 1993, 1994 Jason Williams
    Version: 1.01 (30 May 1994)
    Purpose: High-level window management functions: Create a window
*/


#include <stdlib.h>
#include <string.h>

#include "DeskLib:LinkList.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Template.h"
#include "DeskLib:Event.h"
#include "DeskLib:Window.h"
#include "DeskLib:Screen.h"
#include "DeskLib:Error.h"

#include "WindowDefs.h"


#define ERRBASE 1
#define ERR1 ERRBASE+0
#define ERRMESS1 "Insufficient memory to open window"


linklist_header window_listanchor;


extern window_handle Window_Create(const char *windowname, int maxtitlesize)
{
  windowrec     *record;
  window_block  *windowptr;
  window_handle window;

  windowptr = Template_Clone(windowname, maxtitlesize);
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

