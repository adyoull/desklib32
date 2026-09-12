/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Icon.StartDrag.c
    Author:  Copyright © 1992 Jason Williams
    Version: 1.00 (31 Mar 1992)
    Purpose: Starts the drag operation of any icon (i.e. file icon)
*/


#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Screen.h"
#include "DeskLib:Icon.h"


/*  Note: Use Icon_StartSolidDrag() or DragASprite_DragIcon() to start
 *  dragging an icon with DragASprite
 */

extern void Icon_StartDrag(window_handle window, icon_handle icon)
{
  icon_block   istate;
  window_state wstate;
  drag_block   drag;
  int          left, top;

  Wimp_GetWindowState(window, &wstate);
  Wimp_GetIconState(window, icon, &istate);
  left = wstate.openblock.screenrect.min.x - wstate.openblock.scroll.x;
  top  = wstate.openblock.screenrect.max.y - wstate.openblock.scroll.y;

  drag.window = window;
  drag.type   = drag_FIXEDBOX;

  drag.screenrect.min.x = left + istate.workarearect.min.x;
  drag.screenrect.max.x = left + istate.workarearect.max.x;
  drag.screenrect.min.y = top  + istate.workarearect.min.y;
  drag.screenrect.max.y = top  + istate.workarearect.max.y;

  Screen_CacheModeInfo();
  drag.parent.min.x = 0;
  drag.parent.min.y = 0;
  drag.parent.max   = screen_size;

  Wimp_DragBox(&drag);
}
