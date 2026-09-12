/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Coord.Convert3.c
    Author:  Copyright © 1992 Jason Williams
    Version: 1.00 (22 Mar 1992)
    Purpose: Window <--> Screen coordinate conversion routines
*/


#include "DeskLib:Core.h"
#include "DeskLib:Coord.h"
#include "DeskLib:Wimp.h"


extern void Coord_RectToScreen(wimp_rect *rect, convert_block *convert)
{
  Coord_PointToScreen(&rect->min, convert);
  Coord_PointToScreen(&rect->max, convert);
}
