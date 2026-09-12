/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Window.WholeRedrw.c
    Author:  Copyright © 1995 Sergio Monesi
    Version: 1.00 (26 Feb 1995)
    Purpose: Forces the redraw of a whole window
*/


#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Window.h"


extern os_error *Window_ForceWholeRedraw(window_handle window)
{
 window_redrawblock block;

 block.window=window;
 block.rect.min.x=-65536;
 block.rect.min.y=-65536;
 block.rect.max.x=65536;
 block.rect.max.y=65536;

 return Wimp_ForceRedraw(&block);
}
