/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Drag.DragInit.c
    Author:  Copyright © 1994 Jason Williams
    Version: 1.00 (06 Feb 1994)
    Purpose: Initialise the drag system for use with the Event sublibrary
             If you are not using Event_ then you'll need to set up these
             two handlers in an appropriate manner for yourself.
*/

#include "DeskLib:Core.h"
#include "DeskLib:Wimp.h"

#include "DeskLib:Drag.h"
#include "DeskLib:Event.h"
#include "DeskLib:Handler.h"


extern void Drag_Initialise(BOOL attachNULLhandler)
{
  if (attachNULLhandler)
    Event_Claim(event_NULL, event_ANY, event_ANY, Handler_DragNULL, NULL);

  Event_Claim(event_USERDRAG, event_ANY, event_ANY, Handler_DragFinish, NULL);
}

