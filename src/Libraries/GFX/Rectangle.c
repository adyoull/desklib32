/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    GFX.Rectangle.c
    Author:  Copyright © 1993 Jason Williams
    Version: 1.01 (10 Apr 1993)
    Purpose: Plot command: Rectangle outline
*/

#include "DeskLib:GFX.h"

extern void GFX_Rectangle(int x, int y, int w, int h)
{
   GFX_Move(x, y);
   GFX_DrawBy(0, h);
   GFX_DrawBy(w, 0);
   GFX_DrawBy(0, -h);
   GFX_DrawBy(-w, 0);
}
