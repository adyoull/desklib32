/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    LinkList.ListLength.c
    Author:  Copyright © 1992 Jason Williams
    Version: 1.00 (2 Apr 1992)
    Purpose: Linked list handling functions
*/


#include <stdlib.h>
#include <stdio.h>

#include "DeskLib:LinkList.h"

extern int LinkList_ListLength(linklist_header *anchor)
{
  linklist_header *ptr;
  register int    length = 0;

  ptr = anchor->next;
  while (ptr != NULL)
  {
    length ++;
    ptr = ptr->next;
  }

  return(length);
}
