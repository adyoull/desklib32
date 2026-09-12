/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Event.MRelMsg.c
    Author:  Copyright © 1992 Jason Williams
    Version: 1.00 (16 Mar 1992)
    Purpose: Extension to Event.c to allow routing of specific message types
             to different windows' message handlers.
*/

#include "EMsgDefs.h"



extern BOOL EventMsg_ReleaseMessage(message_action messagetype)
{
  eventmsg_claimrecord *ptr;
  eventmsg_windowrecord *wptr, *next;

  ptr = (eventmsg_claimrecord *) eventmsg__claimanchor.next;

  while (ptr != NULL)
  {
    if (ptr->messagetype == messagetype)
    {
      wptr = (eventmsg_windowrecord *) ptr->windowlist.next;
      while (wptr != NULL)                      /* Release all window claims */
      {
        next = (eventmsg_windowrecord *) wptr->header.next;
        free(wptr);
        wptr = next;
      }

      LinkList_Unlink(&eventmsg__claimanchor, &(ptr->header));
                                                      /* remove message type */
      free(ptr);                                      /* free memory up      */
      return(TRUE);
    }
    ptr = (eventmsg_claimrecord *) ptr->header.next;
  }
  return(FALSE);
}
