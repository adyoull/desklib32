/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Dialog2.OpenPos.c
    Author:  Copyright © 1994 Julian Smith
    Version: 1.03 (13 Mar 1995)
    Purpose: Dialogue box handling
*/


#include "DeskLib:Window.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Dialog2.h"


void	Dialog2_Window_GetWindowOpenPos(
		wimp_point *pos, window_openpos openpos, window_handle window
		)
	/* This is a bit of a cheat - opens the window using Window_Show, looks at where	*/
	/* the window opened, closes the window, and returns the position.			*/
{
window_state	state;

Window_Show( window, openpos);
Wimp_GetWindowState( window, &state);
Wimp_CloseWindow( window);

pos->x = state.openblock.screenrect.min.x;
pos->y = state.openblock.screenrect.max.y;

return;
}

