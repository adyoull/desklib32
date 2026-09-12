/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Coord.Convert1.c
    Author:  Copyright © 1992 Jason Williams
    Version: 1.00 (22 Mar 1992)
    Purpose: Window <--> Screen coordinate conversion routines
*/


#include "DeskLib:Core.h"
#include "DeskLib:Coord.h"
#include "DeskLib:Wimp.h"

/*  Note: The window workarea origin (0,0) is at the TOP LEFT corner of the
 *        work area.
 */


/* --------------------------------------------------------------------------
 * The following routines have been defined in Coord.h as macros...
 *
 * extern int Coord_XToScreen(int x, convert_block *convert)
 * extern int Coord_YToScreen(int x, convert_block *convert)
 *
 */

extern void Coord_PointToScreen(wimp_point *point, convert_block *convert)
{
  point->x = Coord_XToScreen(point->x, convert);
  point->y = Coord_YToScreen(point->y, convert);
}
