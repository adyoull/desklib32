/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Coord.Convert0.c
    Author:  Copyright © 1992 Jason Williams
    Version: 1.01 (26 Apr 1992)
    Purpose: Window <--> Screen coordinate conversion routines
*/


#include "DeskLib:Core.h"
#include "DeskLib:Wimp.h"
#include "DeskLib:Coord.h"

/*  Note: The window workarea origin (0,0) is at the TOP LEFT corner of the
 *        work area.
 */


extern void Coord_WindowOrigin(wimp_point *origin, convert_block *convert)
{
  origin->x = convert->screenrect.min.x - convert->scroll.x;
  origin->y = convert->screenrect.max.y - convert->scroll.y;
}
