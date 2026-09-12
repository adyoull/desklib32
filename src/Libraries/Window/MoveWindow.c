/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Window.MoveWindow.c
    Author:  Copyright © 1995 Sergio Monesi
    Version: 1.00 (26 Jul 1995)
    Purpose: Move window by icon dragging
*/

#include "DeskLib:Wimp.h"
#include "DeskLib:Window.h"
#include "DeskLib:Error.h"

BOOL Window_MoveWindow(event_pollblock *e, void *r)
{
 drag_block block;
 window_state state;

 r = r;

 Wimp_GetWindowState(e->data.mouse.window,&state);

 block.window=e->data.mouse.window;
 block.type=drag_MOVEWINDOW;
 block.screenrect=state.openblock.screenrect;

 Error_Check(Wimp_DragBox(&block));

 return TRUE;
}
