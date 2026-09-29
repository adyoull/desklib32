/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Event.MClaimInit.c
    Author:  Copyright © 1992, 1993, 1994 Jason Williams
    Version: 1.05 (27 Sep 2002)
    Purpose: Extension to Event.c to allow routing of specific message types
             to different windows' message handlers.
*/


#include "EMsgDefs.h"

#include "DeskLib:WimpSWIs.h"

#define ERRBASE 1
#define ERR1 ERRBASE+0
#define ERR2 ERRBASE+1
#define ERRMESS1 "Unable to allocate memory for EventMsg claim."
#define ERRMESS2 "Unable to add Wimp message for EventMsg claim."

linklist_header eventmsg__claimanchor = {NULL, NULL};
static BOOL initialised = FALSE;


static BOOL EventMsg_DispatchMessage(event_pollblock *event, void *reference)
{
  message_action action;
  window_handle  window;
  eventmsg_claimrecord *ptr, *nextptr;
  eventmsg_windowrecord *wptr, *nextwptr;

  UNUSED( reference);

  action = event->data.message.header.action;
  switch(action)
  {
    case message_DATASAVE:
    case message_DATASAVEACK:
    case message_DATALOAD:
      window = event->data.message.data.words[0];
      break;

    case message_HELPREQUEST:
      window = event->data.message.data.helprequest.where.window;
      break;

    default:
      window = event_ANY;
      break;
  }

  ptr = (eventmsg_claimrecord *) eventmsg__claimanchor.next;
  while (ptr != NULL)
  {
    nextptr = (eventmsg_claimrecord *) ptr->header.next;

    if (ptr->messagetype == message_ANY || ptr->messagetype == action)
    {
      wptr = (eventmsg_windowrecord *) ptr->windowlist.next;
      while (wptr != NULL)
      {
        nextwptr = (eventmsg_windowrecord *) wptr->header.next;

        if (window == event_ANY || wptr->window == event_ANY ||
            wptr->window == window)
          if (wptr->handler(event, wptr->reference) == TRUE)
            return(TRUE);

        wptr = nextwptr;
      }
    }
    ptr = nextptr;
  }
  return(FALSE);
}



extern BOOL EventMsg_Claim(message_action messagetype, window_handle window,
                           event_handler handler, void *reference)
{
  eventmsg_claimrecord *ptr;
  eventmsg_windowrecord *wrecord;

  EventMsg_Initialise();

  ptr = (eventmsg_claimrecord *) eventmsg__claimanchor.next;
  while (ptr != NULL)            /* Find claims for this message action type */
  {
    if (ptr->messagetype == messagetype)
      break;

    ptr = (eventmsg_claimrecord *) ptr->header.next;
  }

  if (ptr == NULL)                /* No current claims, so add new claimlist */
  {
    int messagelist[2];

    /* Add the new message code to the list of those we receive from the Wimp */
    messagelist[0] = messagetype;
    messagelist[1] = 0;

    if (Wimp_AddMessages(messagelist) != NULL)
    {
      Error_ReportInternal(ERR2, ERRMESS2);
      return(FALSE);
    }

    ptr = (eventmsg_claimrecord *) malloc(sizeof(eventmsg_claimrecord));
    if (ptr == NULL)
    {
      Error_ReportInternal(ERR1, ERRMESS1);
      return(FALSE);
    }

    LinkList_Init(&(ptr->header));
    ptr->messagetype = messagetype;
    LinkList_Init(&(ptr->windowlist));
    LinkList_AddToHead(&eventmsg__claimanchor, &(ptr->header));
                                          /* Insert new message_handler list */
  }

  wrecord = (eventmsg_windowrecord *) malloc(sizeof(eventmsg_windowrecord));
  if (wrecord == NULL)
  {
    Error_ReportInternal(ERR1, ERRMESS1);
    return(FALSE);
  }

  LinkList_Init(&(wrecord)->header);
  wrecord->window    = window;
  wrecord->handler   = handler;
  wrecord->reference = reference;

 /* Insert new window's handler into the window list for this message action.
  * Specific-window handlers are placed on the front of the list, and non-
  * specific handlers are placed on the end, to give priority to the most
  * specific claim on any message.
  */
  if (window == event_ANY)            /* insert new window's handler in list */
    LinkList_AddToTail(&(ptr->windowlist), &(wrecord->header));
  else
    LinkList_AddToHead(&(ptr->windowlist), &(wrecord->header));

  return(TRUE);
}



extern void EventMsg_Initialise(void)
{
  if (!initialised) {
    Event_Claim(event_USERMESSAGE, event_ANY, event_ANY,
                EventMsg_DispatchMessage, NULL);
    Event_Claim(event_USERMESSAGERECORDED, event_ANY, event_ANY,
                EventMsg_DispatchMessage, NULL);
    Event_Claim(event_USERMESSAGEACK, event_ANY, event_ANY,
                EventMsg_DispatchMessage, NULL);
    initialised = TRUE;
  }
}
