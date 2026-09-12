/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Drag.SetHandler.c
    Author:  Copyright © 1994 Jason Williams
    Version: 1.00 (06 Feb 1994)
    Purpose: Vastly simplify processing of drags-
             attach handlers to process drag updates and drag completion

             NOTE that this system relies on a pair of handlers (for NULL
             and Drag-Finished events) to operate. (See Drag.h)
*/

#include "DeskLib:Drag.h"

drag_handler drag_currentupdate    = NULL;
drag_handler drag_currentcomplete  = NULL;
void         *drag_currentuserdata = NULL;


extern void Drag_SetHandlers(drag_handler uproc, drag_handler cproc,
                             void *userdata)
{
  drag_currentupdate   = uproc;
  drag_currentcomplete = cproc;
  drag_currentuserdata = userdata;
}
