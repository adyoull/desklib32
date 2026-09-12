/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Coord.RectCntain.c
    Author:  Copyright © 1992 Edouard Poor
    Version: 1.00 (22 Mar 1992)
    Purpose: Check if a rectangle lies wholly within another rectangle
*/


#include "DeskLib:Core.h"
#include "DeskLib:Wimp.h"
#include "DeskLib:Coord.h"


extern BOOL Coord_RectContained(wimp_rect *inside, wimp_rect *outside)
    {
    if(inside->min.x < outside->min.x) return(FALSE);
    if(inside->max.x > outside->max.x) return(FALSE);
    if(inside->min.y < outside->min.y) return(FALSE);
    if(inside->max.y > outside->max.y) return(FALSE);
    return(TRUE);
    }
