/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    LinkList.c
    Author:  Copyright © 1992 Jason Williams
    Version: 1.02 (16 Mar 1992)
    Purpose: Linked list handling functions
*/


#include <stdlib.h>
#include <stdio.h>

#include "DeskLib:LinkList.h"

extern void LinkList_AddToHead(linklist_header *anchor, linklist_header *item)
{
  linklist_header *oldfirst;

  oldfirst       = anchor->next;
  item->next     = oldfirst;                                /* link in next  */
  item->previous = NULL;                                    /* First in list */

  if (oldfirst == NULL)
    anchor->previous = item;              /* First item, so it is 1st & last */
  else
    oldfirst->previous = item;                              /* reverse link  */
  anchor->next = item;                                      /* forward link  */
}
