/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    LinkList.InsertA.c
    Author:  Copyright © 1992 Jason Williams
    Version: 1.02 (16 Mar 1992)
    Purpose: Linked list handling functions
*/


#include <stdlib.h>
#include <stdio.h>

#include "DeskLib:LinkList.h"

extern void LinkList_InsertAfter(linklist_header *anchor,
                                 linklist_header *pos,
                                 linklist_header *item)
{
  linklist_header *oldnext;

  oldnext = pos->next;

  if (oldnext == NULL)                             /* Insert at tail of list */
    LinkList_AddToTail(anchor, item);
  else
  {
    oldnext->previous = item;                      /* Link to next in list   */
    item->next        = oldnext;

    pos->next         = item;                      /* Link "pos" in as prev  */
    item->previous    = pos;
  }
}
