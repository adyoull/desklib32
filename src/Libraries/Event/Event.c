/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Event.c
    Author:  Copyright © 1992, 1993, 1994 John Winters
             (mucked about by Jason Williams and Tim Browse)
             (and by Julian Smith)
    Version: 1.11 (18 Nov 2002)
    Purpose: High-level WIMP event dispatch to a hierarchy of user event
             handling functions.

    PJC:     Added Event_Initialise3 and allowed support for the
             NonZero Poll Word event

    JCW:     Fixed incorrect releasing of NULL events on event_ANY release.

    JS:      Changed external globals to work with Straylight, and moved
             event_taskhandle into a separate .c file

    AS:      Removed the DLL stuff - it's unneeded gunk now.
*/

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "DeskLib:Error.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Time.h"

#include "EventDefs.h"


/* Error message definitions.
   These are ALL the messages produced by this module, and have been defined
   here so that it is easier for you to alter them or replace them with
   msgtrans tags.
   Error numbers are also included: Currently, the error number is unused
   so they are defined starting from ERRBASE. If you wish to use the error
   number in your Error reporting code, then you will have to set ERRBASE to
   a suitable value to ensure that all errors produced have unique numbers.
*/
#define ERRBASE  1
#define ERR1     ERRBASE+0
#define ERRMESS1 "Can't allocate memory for an icon record"
#define ERR2     ERRBASE+1
#define ERRMESS2 "Can't allocate memory for a window record"
#define ERR3     ERRBASE+2
#define ERRMESS3 "Can't allocate memory to record an event claim"
#define ERR4     ERRBASE+3
#define ERRMESS4 "Attempt to claim invalid event type"
#define ERR5     ERRBASE+4
#define ERRMESS5 "Can't trace event claim"


/* JS 22 Mar 1995
Need to make these global non-static variables visible to this file.
Event.h #defines them so that any reference to 'event_mask' (for eg)
actually calls the function 'Event__Ref_mask', so that the DLL
application start-time is correctly set so that the correct instance
data is returned.
*/

event_pollmask  event_mask = { 0 };
event_pollblock event_lastevent;
task_handle     event_taskhandle  = 0;
unsigned int    event_wimpversion = 0;


static int usagecounts[wimp_NUMBEROFEVENTS]= {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


/*
   PJC: needed to remove the 'const' declaration so that
   the list can be altered on the fly
*/
static event_class eventclass [event_MAXEVENTS] =
{
    notrelated,     /* NULL             */
    windowrelated,  /* redraw           */
    windowrelated,  /* open window      */
    windowrelated,  /* close window     */
    windowrelated,  /* pointer leave    */
    windowrelated,  /* pointer enter    */
    iconrelated,    /* Mouse click      */
    notrelated,     /* drag finished    */
    iconrelated,    /* keypress         */
    notrelated,     /* menu selection   */
    windowrelated,  /* scroll           */
    iconrelated,    /* lose caret       */
    iconrelated,    /* gain caret       */
    notrelated,     /* nonzero pollword */
    nonexistent,    /* 14               */
    nonexistent,    /* 15               */
    nonexistent,    /* 16               */
    notrelated,     /* Message          */
    notrelated,     /* Message recorded */
    notrelated      /* Message ack      */
};

static linklist_header catchalls = {NULL, NULL};

static linklist_header eventqueues[event_MAXEVENTS] =
{
  {NULL, NULL},
  {NULL, NULL},
  {NULL, NULL},
  {NULL, NULL},
  {NULL, NULL},
  {NULL, NULL},
  {NULL, NULL},
  {NULL, NULL},
  {NULL, NULL},
  {NULL, NULL},
  {NULL, NULL},
  {NULL, NULL},
  {NULL, NULL},
  {NULL, NULL},
  {NULL, NULL},
  {NULL, NULL},
  {NULL, NULL},
  {NULL, NULL},
  {NULL, NULL},
  {NULL, NULL}
};


static linklist_header window_anchor = {NULL, NULL};
static void *currentscan = NULL;




/* prototypes for the 3 handler functions used in the event_handlers list */
static BOOL DispatchIconEvents(event_pollblock *event, void *reference);
static BOOL DispatchWindowEvents(event_pollblock *event, void *reference);
static BOOL DispatchMiscEvents(event_pollblock *event, void *reference);

static event_handler event_handlers[wimp_NUMBEROFEVENTS] = {
    DispatchMiscEvents,
    DispatchWindowEvents,
    DispatchWindowEvents,
    DispatchWindowEvents,
    DispatchWindowEvents,
    DispatchWindowEvents,
    DispatchIconEvents,
    DispatchMiscEvents,
    DispatchIconEvents,
    DispatchMiscEvents,
    DispatchWindowEvents,
    DispatchIconEvents,
    DispatchIconEvents,
    NULL,
    NULL,
    NULL,
    NULL,
    DispatchMiscEvents,
    DispatchMiscEvents,
    DispatchMiscEvents
};


/* which events can be masked out? */
static const char event_masks[event_MAXEVENTS] = {
  TRUE,          /* Null             */
  FALSE,         /* Redraw Window    */
  FALSE,         /* Open Window      */
  FALSE,         /* Close Window     */
  TRUE,          /* Pointer Leaving  */
  TRUE,          /* Pointer Entering */
  FALSE,         /* Click            */
  FALSE,         /* Drag finish      */
  FALSE,         /* Key              */
  FALSE,         /* Menu Selection   */
  FALSE,         /* Scroll           */
  TRUE,          /* Lose caret       */
  TRUE,          /* Gain caret       */
  TRUE,          /* NonZero PollWord */
  FALSE,         /* Reserved         */
  FALSE,         /* Reserved         */
  FALSE,         /* Reserved         */
  FALSE,         /* Message          */
  TRUE,          /* Message Recorded */
  TRUE           /* Message Ack      */
};


/* =========================================================================
   Functions
   ========================================================================= */


static void IncrementUsage(event_type event)
{
  if ((usagecounts[event])++ == 0)      /* increment count: if not used b4...*/
  {
    event_mask.value &= ~(1 << event);  /* then stop masking event out       */

    /* PJC: if NZPW event type, also set bit 22 */
    if (event == event_NONZEROPOLLWORD)
      event_mask.data.r3ispollwordptr = TRUE;
  }
}


static void DecrementUsage(event_type event)
{
  if (--(usagecounts[event]) == 0)      /* decrement count: if not used now..*/
  {
    if (event_masks[event])             /* then mask the event out           */
      event_mask.value |= 1 << event;

    /* PJC: if NZPW event type, also clear bit 22 */
    if (event == event_NONZEROPOLLWORD)
      event_mask.data.r3ispollwordptr = FALSE;
  }
}


static event_iconrecord *CreateIconRecord(event_windowrecord *windowrecord,
                                          icon_handle        icon)
{
  event_iconrecord *current;

  current = malloc(sizeof(event_iconrecord));
  if (current == NULL)
    Error_ReportInternal(ERR1, ERRMESS1);
  else
  {
    current->icon = icon;
    LinkList_Init(&(current->claimlist));
    LinkList_AddToTail(&(windowrecord->iconlist), &(current->header));
  }

  return(current);
}



static event_windowrecord *CreateWindowRecord(window_handle window)
{
  event_windowrecord *current;

  current = malloc(sizeof(event_windowrecord));
  if (current == NULL)
    Error_ReportInternal(ERR2, ERRMESS2);
  else
  {
    current->window = window;
    LinkList_Init(&(current->iconlist));
    LinkList_Init(&(current->claimlist));
    LinkList_AddToTail(&window_anchor, &(current->header));
  }
  return(current);
}



static event_iconrecord *FindIcon(event_windowrecord *windowrecord,
                                  icon_handle        icon)
{
  event_iconrecord *current;

  current = LinkList_FirstItem(&(windowrecord->iconlist));
  while (current != NULL)
  {
    if (current->icon == icon)
      break;
    current = LinkList_NextItem(&(current->header));
  }
  return(current);
}



static event_windowrecord *FindWindow(window_handle window)
{
  event_windowrecord *current;

  current = LinkList_FirstItem(&window_anchor);
  while (current != NULL)
  {
    if (current->window == window)
      break;
    current = LinkList_NextItem(&(current->header));
  }

  return(current);
}



extern BOOL Event_Claim(event_type eventtype,
                        window_handle window,  icon_handle icon,
                        event_handler handler, void *reference)
{
  event_claimrecord  *record;
  event_iconrecord   *iconrecord;
  event_windowrecord *windowrecord;
  int                index;
  BOOL               result;

  result = FALSE;
  if ((eventtype == event_ANY) ||
       ((eventtype >= 0) && (eventtype < wimp_NUMBEROFEVENTS) &&
        (eventclass[eventtype] != nonexistent)))
  {
    record = malloc(sizeof(event_claimrecord));
    if (record == NULL)
    {
      Error_ReportInternal(ERR3, ERRMESS3);
      return(TRUE);
    }

    record->eventtype = eventtype;
    record->handler   = handler;
    record->reference = reference;

    if (window == event_ANY)
    {
      if (eventtype == event_ANY)
      {
        /* Any events to any window (lowest level of hierarchy) */
        LinkList_AddToHead(&catchalls, &(record->header));
        result = TRUE;

        for (index = 1; index < event_MAXEVENTS; index++)
          if (eventclass[index] != nonexistent)
            IncrementUsage( (event_type) index);
      }
      else
      {
        /* Specific event to any window */
        LinkList_AddToHead(&(eventqueues[eventtype]), &(record->header));
        result = TRUE;

        IncrementUsage(eventtype);
      }
    }
    else
    {
      /* Claims for specific windows... */
      windowrecord = FindWindow(window);/* Find/Make claim record for window */
      if (windowrecord == NULL)
        windowrecord = CreateWindowRecord(window);

      if (windowrecord != NULL)
      {
        if (icon == event_ANY)
        {
          /* Specific window, any icon */
          if (eventtype == event_ANY)
          {
            /* Specific window, any icon, any event */
            LinkList_AddToTail(&(windowrecord->claimlist), &(record->header));
            result = TRUE;

            /* NULL events are not included in "event_ANY", so start from 1 */
            for (index = 1; index < event_MAXEVENTS; index++)
              if (eventclass[index] != nonexistent)
                IncrementUsage ( (event_type) index);
          }
          else
          {
            /* specific window, any icon, specific event */
            LinkList_AddToHead(&(windowrecord->claimlist), &(record->header));
            result = TRUE;

            IncrementUsage(eventtype);
          }

        }
        else
        {
          /* Specific window, specific icon */
          iconrecord = FindIcon(windowrecord, icon);
          if (iconrecord == NULL)
              iconrecord = CreateIconRecord(windowrecord, icon);

          if (iconrecord != NULL)
          {
            if (eventtype == event_ANY)
            {
              /* specific window, specific icon, any event */
              LinkList_AddToTail(&(iconrecord->claimlist),&(record->header));
              result = TRUE;

              /* NULL events are not included in "event_ANY" so start from 1 */
              for (index = 1; index < event_MAXEVENTS; index++)
                if (eventclass[index] != nonexistent)
                  IncrementUsage( (event_type) index);
            }
            else
            {
              /* specific window, specific icon, specific event */
              LinkList_AddToHead(&(iconrecord->claimlist), &(record->header));
              result = TRUE;

              IncrementUsage(eventtype);
            }
          }
        }
      }
    }
    if (result == FALSE)
        free (record);
  }
  else
    Error_ReportInternal(ERR4, ERRMESS4);

  return(result);
}



static void LoseClaim(linklist_header *listheader,
                      event_claimrecord *claimrecord)
{
  int index;

  if (claimrecord->eventtype == event_ANY)
  {
    /* "event_ANY" does not include event_NULL, so start from 1 */
    for (index = 1; index < wimp_NUMBEROFEVENTS; index++)
      if (eventclass[index] != nonexistent)
              DecrementUsage( (event_type) index);
  }
  else
    DecrementUsage(claimrecord->eventtype);

  if (claimrecord == currentscan)
    currentscan = NULL; /* Make other routines aware we have deleted the item
                           that they are currently scanning                  */

  LinkList_Unlink(listheader, &(claimrecord->header));

  free(claimrecord);
}



static BOOL RemoveClaim(linklist_header *header,
                        event_type      eventtype,
                        event_handler   handler, void *reference,
                        int             *lastone)
/* Returns TRUE if it found and removed the claim.
 * If it did, it also returns a flag: lastone == TRUE if no more claims left
 * on this event
 */
{
  event_claimrecord *claimrecord;
  BOOL result;

  result = FALSE;
  claimrecord = LinkList_FirstItem(header);

  while (claimrecord != NULL)
  {
    if ((claimrecord->eventtype == eventtype) &&
        (claimrecord->handler   == handler)   &&
        (claimrecord->reference == reference))
    {
      LoseClaim(header, claimrecord);
      result = TRUE;
      break;
    }

    claimrecord = LinkList_NextItem(&(claimrecord->header));
  }

  if ((result) && (lastone != NULL))
    *lastone = (LinkList_FirstItem(header) == NULL);    /* return the result */

  return(result);
}



static void LoseIcon(linklist_header *list, event_iconrecord *iconrecord)
{
  event_claimrecord *claimrecord;

  while ((claimrecord = LinkList_FirstItem(&(iconrecord->claimlist))) != NULL)
    LoseClaim(&(iconrecord->claimlist), claimrecord);

  LinkList_Unlink(list, &(iconrecord->header));
  free(iconrecord);
}



static void LoseWindow(event_windowrecord *windowrecord)
{
  LinkList_Unlink(&window_anchor, &(windowrecord->header));
  free(windowrecord);
}



extern BOOL Event_Release(event_type event,
                          window_handle  window, icon_handle icon,
                          event_handler handler, void *reference)
{
  event_windowrecord *windowrecord;
  event_iconrecord *iconrecord;
  int  lastone;
  BOOL result;

  result = FALSE;

  if (window == event_ANY)
  {
    if (event == event_ANY)
      result = RemoveClaim(&catchalls, event, handler, reference, NULL);
    else
      result = RemoveClaim (&(eventqueues[event]),
                               event, handler, reference, NULL);
  }
  else
  {
    windowrecord = FindWindow(window);
    if (windowrecord != NULL)
    {
      if (icon == event_ANY)
        result = RemoveClaim(&(windowrecord->claimlist),
                                 event, handler, reference, NULL);
      else
      {
        iconrecord = FindIcon(windowrecord, icon);
        if (iconrecord != NULL)
        {
          if (RemoveClaim(&(iconrecord->claimlist),
                                      event, handler, reference, &lastone))
          {
            result = TRUE;
            if (lastone)
              LoseIcon(&(windowrecord->iconlist), iconrecord);
          }
        }
      }

      if (result)
      {
        if ((LinkList_FirstItem(&(windowrecord->iconlist)) == NULL) &&
            (LinkList_FirstItem(&(windowrecord->claimlist)) == NULL))
          LoseWindow(windowrecord);
      }
    }
  }

  if (!result)
    Error_ReportInternal(ERR5, ERRMESS5);

  return (result) ;
}



extern void Event_ReleaseWindow(window_handle window)
{
  event_claimrecord  *claimrecord;
  event_iconrecord   *iconrecord;
  event_windowrecord *windowrecord;

  windowrecord = FindWindow(window);
  if (windowrecord != NULL)
  {
    while ((claimrecord = LinkList_FirstItem(&(windowrecord->claimlist)))
           != NULL)
      LoseClaim(&(windowrecord->claimlist), claimrecord) ;

    while ((iconrecord = LinkList_FirstItem(&(windowrecord->iconlist)))
           != NULL)
      LoseIcon(&(windowrecord->iconlist), iconrecord);

    LoseWindow(windowrecord);
  }
}



/* Event dispatchers. These functions take in an event, and find the highest-
 * priority user event handler, which is then called to process the event
 */

static BOOL ScanMiscQueue(linklist_header *queue, event_pollblock *event)
{
  event_claimrecord *current;

  current = LinkList_FirstItem(queue);
  while (current != NULL)
  {
    currentscan = current;
    if (current->handler(event, current->reference))
      return(TRUE);                    /* User handler has handled the event */

    if (currentscan == current)
      current = LinkList_NextItem(&(current->header));
    else
      current = LinkList_FirstItem(queue);/* item deleted out from under us! */
  }
  return(FALSE);
}



static BOOL DispatchMiscEvents(event_pollblock *event, void *reference)
/* Processes any general event that either is non-window-specific, or has not
 * been processed by any higher-level handlers. If NO handler at this level
 * manages to handle the event, then only default action is to check for the
 * QUIT message, and quit if necessary.
 */
{
  BOOL dummy;
  linklist_header *queue;

  UNUSED( reference);

  queue = &eventqueues[event->type];          /* Try specific event handlers */
  dummy = ScanMiscQueue(queue, event);

  if (!dummy)
    dummy = ScanMiscQueue(&catchalls, event);      /* Try catch-all handlers */

  /* User handlers haven't processed event, so check for QUIT message */
  if (!dummy)
    if (((event->type == event_USERMESSAGE) ||
         (event->type == event_USERMESSAGERECORDED)) &&
         (event->data.message.header.action == message_QUIT))
      exit(0);          /* Just die. atexit() handlers should tidy up for us */

  return(TRUE);   /* Always assume we have handled the event, because we are
                     the last event handler that will ever be called...      */
}



static BOOL ScanWindowQueue(linklist_header *queue, event_pollblock *event)
{
  event_claimrecord *current;

  current = LinkList_FirstItem(queue);
  while (current != NULL)
  {
    currentscan = current ;
    if ((current->eventtype == event->type) ||
        (current->eventtype == event_ANY))
      if (current->handler(event, current->reference))
        return(TRUE);                      /* User handler has handled event */

    if (currentscan == current)
      current = LinkList_NextItem(&(current->header));
    else
      current = LinkList_FirstItem(queue);/* item deleted out from under us! */
  }
  return(FALSE);
}



static BOOL DispatchIconEvents(event_pollblock *event, void *reference)
{
  icon_handle        icon;
  event_iconrecord   *iconrecord;
  window_handle      window;
  event_windowrecord *windowrecord;

  UNUSED( reference);

  if (event->type == event_CLICK)
    window = event->data.mouse.window;
  else
    window = event->data.openblock.window;

  windowrecord = FindWindow(window);
  if (windowrecord != NULL)
  {
    switch (event->type)
    {
      case event_CLICK:
        icon = event->data.mouse.icon;
        break;

      case event_KEY:
        icon = event->data.key.caret.icon;
        break;

      case event_LOSECARET:
      case event_GAINCARET:
        icon = event->data.caret.icon;
        break;

      default:
        icon = -1;
        break;
    }

    if (icon != -1)
    {
      iconrecord = FindIcon(windowrecord, icon);
      if (iconrecord != NULL)
        if (ScanWindowQueue(&(iconrecord->claimlist), event))
          return(TRUE);  /* event handled */
    }

    if (ScanWindowQueue(&(windowrecord->claimlist), event))
      return(TRUE);  /* event handled */
  }

/* Unable to find a highlevel handler for this event, so fallback to the
 * low-level handlers...
 */

  return(DispatchMiscEvents(event, NULL));
}



static BOOL DispatchWindowEvents(event_pollblock *event, void *reference)
{
  window_handle      window;
  event_windowrecord *windowrecord;

  UNUSED( reference);

  window = event->data.openblock.window;
  windowrecord = FindWindow(window);

  if (windowrecord != NULL)
    if (ScanWindowQueue(&(windowrecord->claimlist), event))
      return(TRUE);  /* Window handler has dealt with the event */

/* Unable to find a highlevel handler for this event, so fallback to the
 * low-level handlers...
 */

  return(DispatchMiscEvents(event, NULL));
}


static void Event__ExitFunction(void)
{
  if (event_taskhandle != 0)
    Wimp_CloseDown(event_taskhandle);
}




/* And now, ladies and gentlemen, the routines you have all been waiting
 * for... I give you... the amazing Eventi brothers...
 */

extern void Event_Initialise3(const char *taskname, int version, int *messages)
{
  int index;

  atexit(Event__ExitFunction);

  for (index = 0; index < event_MAXEVENTS; index++)
  {
    if (event_masks[index])
      event_mask.value |= 1 << index;
  }

  if (version >= 300)
  {
    eventclass[event_NONZEROPOLLWORD] = notrelated;
    event_handlers[event_NONZEROPOLLWORD] = DispatchMiscEvents;
  }

  event_wimpversion = version;


  Error_CheckFatal(Wimp_Initialise(&event_wimpversion,
                                     taskname, &event_taskhandle, messages));


  strncpy(event_taskname, taskname, 39);

  event_taskname[39] = 0;
}


extern void Event_InitNested(const char *taskname)
{
  Event_Initialise3(taskname, 380, NULL);
}


extern void Event_Process(event_pollblock *event)
{
  event_handler handler;

  if (event->type < event_MAXEVENTS) {
    handler = (event_handler) (event_handlers[event->type]);
    if (handler) handler(event, NULL);
  }
}

extern void Event_Poll(void)
{
  Wimp_Poll(event_mask, &event_lastevent);
  Event_Process(&event_lastevent);
}

/*
 * Takes a delay in cs, which is the earliest time after this call that
 * we want to receive a Null Reason event.
 */
extern void Event_PollIdle(unsigned int delay)
{
  Wimp_PollIdle(event_mask, &event_lastevent, Time_Monotonic() + delay);
  Event_Process(&event_lastevent);
}

extern void Event_CloseDown(void)
{
  exit(0);  /* Wimp_CloseDown is handled by the ExitFunction */
}
