/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    LinkList.InsertB.c
    Author:  Copyright © 1992 Jason Williams
    Version: 1.02 (16 Mar 1992)
    Purpose: Linked list handling functions
*/


#include <stdlib.h>
#include <stdio.h>

#include "DeskLib:LinkList.h"


extern void LinkList_InsertBefore(linklist_header *anchor,
                                  linklist_header *pos,
                                  linklist_header *item)
{
  linklist_header *oldprev;

  oldprev = pos->previous;

  if (oldprev == NULL)                             /* Insert at head of list */
    LinkList_AddToHead(anchor, item);
  else
  {
    oldprev->next  = item;                         /* Link to prev in list   */
    item->previous = oldprev;

    pos->previous  = item;                         /* Link "pos" in as next  */
    item->next     = pos;
  }
}
