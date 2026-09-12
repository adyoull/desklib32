/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Event.EMsgDefs.h
    Author:  Copyright © 1992 Jason Williams
    Version: 1.01 (08 Jul 1995)
    Purpose: Extension to Event.c to allow routing of specific message types
             to different windows' message handlers.
    Mods:    JS - Added prototype for eventmsg__claimanchor.
*/

#include <stdlib.h>
#include "DeskLib:LinkList.h"
#include "DeskLib:Error.h"
#include "DeskLib:Event.h"
#include "DeskLib:EventMsg.h"


typedef struct
{
  linklist_header header;
  message_action  messagetype;
  linklist_header windowlist;
} eventmsg_claimrecord;



typedef struct
{
  linklist_header header;
  window_handle   window;
  event_handler   handler;
  void            *reference;
} eventmsg_windowrecord;


extern linklist_header eventmsg__claimanchor;
