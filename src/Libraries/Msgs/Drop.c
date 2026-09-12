/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Msgs.Drop.c
    Author:  Copyright © 1992 Jason Williams
    Version: 1.00 (08 Apr 1992)
    Purpose: MessageTrans-like message handling functions.
             (If you want MessageTrans, use the SWI interface, if you want
             high-level message handling, use this code...)
*/


#include <stdlib.h>

#include "MsgsDefs.h"
#include "DeskLib:Msgs.h"



extern void Msgs_DropGroup(const char *grouptag)
{
  msgdefptr ptr, next, last = NULL;

  ptr = msgs_grouplist;            /* Try to find group */
  while (ptr != NULL)
  {
    if (Msgs__MatchToken(grouptag, ptr->tag, TRUE))
      break;

    last = ptr;
    ptr = ptr->next;
  }

  if (ptr == NULL)                  /* group not found   */
    return;

  if (ptr == msgs_grouplist)        /* unlink from list  */
    msgs_grouplist = ptr->next;
  else
    last->next = ptr->next;

  next = ptr->data.taglist;         /* Kill group record */
  free(ptr);
  ptr = next;

  while (ptr != NULL)               /* Kill message list */
  {
    if (ptr->data.text != NULL)
      free(ptr->data.text);

    next = ptr->next;
    free(ptr);
    ptr = next;
  }
}
