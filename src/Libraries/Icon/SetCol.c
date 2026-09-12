/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Icon.SetCol.c
    Author:  Copyright © 1994 Jason Williams
    Version: 1.00 (22 March 1994)
    Purpose: Set-colour routines for non-outline-font icons
*/

#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"


extern void Icon_SetForeColour(window_handle window, icon_handle icon,
                               int wimpcolour)
{
  Wimp_SetIconState(window, icon, (wimpcolour & 0x0f) << 24, 0x0f000000);
}


extern void Icon_SetBackColour(window_handle window, icon_handle icon,
                               int wimpcolour)
{
  Wimp_SetIconState(window, icon, (wimpcolour & 0x0f) << 28, 0xf0000000);
}

