/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Coord.RectsOvlap.c
    Author:  Copyright © 1992 Jason Williams
    Version: 1.00 (22 Mar 1992)
    Purpose: Check if two rectangles overlap (rects intersect, or one rect
             entirely contained by the other)
*/


#include "DeskLib:Core.h"
#include "DeskLib:Wimp.h"
#include "DeskLib:Coord.h"


extern BOOL Coord_RectsOverlap(wimp_rect *rect1, wimp_rect *rect2)
{
  if (rect1->min.y > rect2->max.y)  return(FALSE);
  if (rect1->max.y < rect2->min.y)  return(FALSE);

  if (rect1->min.x > rect2->max.x)  return(FALSE);
  if (rect1->max.x < rect2->min.x)  return(FALSE);
  return(TRUE);
}
