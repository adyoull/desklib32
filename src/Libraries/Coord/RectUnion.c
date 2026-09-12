/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Coord.RectUnion.c
    Author:  Copyright © 1994 Tim Browse
    Version: 1.00 (03 Mar 1994)
    Purpose: Find the union of two rectangles.
*/


#include "DeskLib:Core.h"
#include "DeskLib:Wimp.h"
#include "DeskLib:Coord.h"


extern void Coord_RectUnion(wimp_rect *dest, wimp_rect *src1, wimp_rect *src2)
{
  dest->min.x = MIN(src1->min.x, src2->min.x);
  dest->min.y = MIN(src1->min.y, src2->min.y);
  dest->max.x = MAX(src1->max.x, src2->max.x);
  dest->max.y = MAX(src1->max.y, src2->max.y);
}
