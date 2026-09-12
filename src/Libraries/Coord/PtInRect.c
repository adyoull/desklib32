/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Coord.PtInRect.c
    Author:  Copyright © 1992 Edouard Poor
    Version: 1.01 (13 Jul 1993)
    Purpose: Check if a point lies within a rectangle
*/


#include "DeskLib:Core.h"
#include "DeskLib:Wimp.h"
#include "DeskLib:Coord.h"


BOOL Coord_PointInRect(wimp_coord *point, wimp_rect *rectangle)
    {
    if(point->x < rectangle->min.x) return(FALSE);
    if(point->x > rectangle->max.x) return(FALSE);
    if(point->y < rectangle->min.y) return(FALSE);
    if(point->y > rectangle->max.y) return(FALSE);
    return(TRUE);
    }
