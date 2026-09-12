/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    EventDefs.h
    Author:  Copyright © 1992 Jason Williams
    Version: 1.00 (13 Mar 1992)
    Purpose: High-level WIMP event dispatch to a hierarchy of user event
             handling functions - internal structure definitions
*/


#ifndef __dl_eventdefs_h
#define __dl_eventdefs_h

#ifndef __dl_core_h
#include "DeskLib:Core.h"
#endif

#ifndef __dl_linklist_h
#include "DeskLib:LinkList.h"
#endif

#ifndef __dl_wimp_h
#include "DeskLib:Wimp.h"
#endif

#ifndef __dl_event_h
#include "DeskLib:Event.h"
#endif


#define event_MAXEVENTS 20


typedef struct
{
  linklist_header header;
  event_type      eventtype;
  event_handler   handler;
  void            *reference;
} event_claimrecord;

typedef char event_class;
#define  nonexistent   0
#define  iconrelated   1
#define  windowrelated 2
#define  notrelated    3

typedef struct
{
  linklist_header header;
  icon_handle     icon;
  linklist_header claimlist;
} event_iconrecord;

typedef struct
{
  linklist_header header;
  window_handle   window;
  linklist_header iconlist;
  linklist_header claimlist;
} event_windowrecord;

#endif
