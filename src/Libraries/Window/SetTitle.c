/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Window.c.SetTitle
    Author:  Copyright © 1993 Jason Williams
    Version: 1.00 (28 Jun 1993)
             1.01 (28 Oct 2007) Uses new nested-wimp ForceRedraw to update title, if available.
    Purpose: Set the text in a window's titlebar, and force a redraw of
             the *correct* region of the screen to update it.
             (i.e. it even works with Edouard Poor's MegaWindow toolsprites!)
*/

#include <string.h>

#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Window.h"
#include "DeskLib:SWI.h"
#include "DeskLib:Event.h"


extern void Window_SetTitle(window_handle window, const char *title)
{
  window_info        info;
  window_redrawblock redraw;
  window_outline     outline;
  char               *buffer;
  int                bufflen;

  /* Get the wimp information for this window */
  Window_GetInfo(window, &info);

  if (!info.block.titleflags.data.indirected)
    return;                /* We can't set the title if it isn't indirected! */

  buffer  = info.block.title.indirecttext.buffer;
  bufflen = info.block.title.indirecttext.bufflen - 1;

  /* Copy the given string into the window's title bar string */
  strncpy(buffer, title, bufflen);
  buffer[bufflen] = 0;              /* And ensure it is correctly terminated */

  if (info.block.flags.data.open)   /* *IF* window open, redraw the titlebar */
  {
    if (event_wimpversion < 380)
    {
      /* Get the position of the window on screen */
      outline.window = window;
      Wimp_GetWindowOutline(&outline);

      /*  Force a redraw of the title bar - it is the rectangle that lies
       *  between the window outline as returned by Wimp_GetWindowOutline and
       *  the window screenrect as returned by Wimp_GetWindowInfo
       */
      redraw.window     = -1;                        /* invalidate screen area */
      redraw.rect       = outline.screenrect;
      redraw.rect.min.y = info.block.screenrect.max.y;
      Wimp_ForceRedraw(&redraw);
    }
    else
    {
      /* Nested wimp provides direct means of updating title bar */
      SWI(3, 0, 0x400D1/*Wimp_ForceRedraw*/, window, 0x4B534154/*"TASK"*/, 3);
    }
  }
}
