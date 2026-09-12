/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Event.MReleaseS.c
    Author:  Copyright © 1992, 2003 Jason Williams, Antony Sidwell
    Version: 1.01 (24 Jun 2003)
    Purpose: Releases a specific EventMsg handler based on *all* the
             parameters with which it was claimed.
*/

#include "EMsgDefs.h"



extern BOOL EventMsg_ReleaseSpecific(message_action messagetype, window_handle window, event_handler handler, void *reference)
{
  eventmsg_claimrecord *ptr;
  eventmsg_windowrecord *wptr;

  ptr = (eventmsg_claimrecord *) eventmsg__claimanchor.next;

  while (ptr != NULL)                           /* Search all message claims */
  {
    if (ptr->messagetype == messagetype)
    {
      wptr = (eventmsg_windowrecord *) ptr->windowlist.next;
      while (wptr != NULL)                      /* Search all window claims  */
      {
        if (wptr->window == window &&
            wptr->handler == handler &&
            wptr->reference == reference)
        {                         /* Found claim for window+handler+ref combo */
          LinkList_Unlink(&(ptr->windowlist), &(wptr->header));
          free(wptr);

          if (ptr->windowlist.next == NULL)          /* m-list is now empty */
          {
            LinkList_Unlink(&eventmsg__claimanchor, &(ptr->header));
                                                     /* remove message type */
            free(ptr);                               /* free memory up      */
          }
          
          /* We have removed a handler */
          return TRUE;
        }
        wptr = (eventmsg_windowrecord *) wptr->header.next;
      }

    }
    ptr = (eventmsg_claimrecord *) ptr->header.next;
  }

  return FALSE;
}
