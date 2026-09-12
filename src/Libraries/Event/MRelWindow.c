/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Event.MRelWindow.c
    Author:  Copyright © 1992 Jason Williams
    Version: 1.00 (16 Mar 1992)
    Purpose: Extension to Event.c to allow routing of specific message types
             to different windows' message handlers.
*/

#include "EMsgDefs.h"



extern int EventMsg_ReleaseWindow(window_handle window)
{
  eventmsg_claimrecord *ptr, *nextmess;
  eventmsg_windowrecord *wptr, *next;
  int result = 0;

  ptr = (eventmsg_claimrecord *) eventmsg__claimanchor.next;

  while (ptr != NULL)                           /* Search all message claims */
  {
    wptr = (eventmsg_windowrecord *) ptr->windowlist.next;
    while (wptr != NULL)                        /* Search all window claims  */
    {
      next = (eventmsg_windowrecord *) wptr->header.next;
      if (wptr->window == window)          /* found a claim for this window  */
      {
        LinkList_Unlink(&(ptr->windowlist), &(wptr->header));
        free(wptr);
        result += 1;                       /* Count of window claims removed */
      }
      wptr = next;
    }

    nextmess = (eventmsg_claimrecord *) ptr->header.next;
    if (ptr->windowlist.next == NULL)                 /* m-list is now empty */
    {
      LinkList_Unlink(&eventmsg__claimanchor, &(ptr->header));
                                                      /* remove message type */
      free(ptr);                                      /* free memory up      */
    }

    ptr = nextmess;
  }

  return(result);
}
