/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Window.GetInfo.c
    Author:  Copyright © 1992 Jason Williams, Cy Booker
    Version: 1.01 (26 Nov 1994)
    Purpose: High-level window management functions: Get Window info
    Mods:    1.01 (26 Nov 1994) Jason Williams
             1.02 (05 Mar 1995) Cy Booker
                                now returns an (os_error *)
*/

#include "DeskLib:Window.h"
#include "DeskLib:WimpSWIs.h"


extern os_error *Window_GetInfo(window_handle window, window_info *result)
/* Returns just the window part of the window_info block (strips icon defn.s)
 */
{
    result->window = window;
    return Wimp_GetWindowInfo((window_info *)(void *)(((char *) result) + 1));
}
