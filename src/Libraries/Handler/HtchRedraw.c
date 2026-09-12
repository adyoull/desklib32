/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Handler.HtchRedraw.c
    Author:  Copyright © 1993 Mark H. Wilkinson
    Version: 1.00 (28 Sept 1993)
    Purpose: Hatch the work area of a window. Useful when testing
             a program before you get round to writing your own redraw
             routines so you can see that something is happening.
*/


#include "DeskLib:WimpSWIs.h"
#include "DeskLib:GFX.h"
#include "DeskLib:Handler.h"

#define hatch_size 48

extern BOOL Handler_HatchRedraw(event_pollblock *event, void *reference)
{
  window_redrawblock redraw;
  BOOL more;
  int origin_x, origin_y;
  int xstart, height, xcorr, ycorr, x;

  UNUSED( reference);

  redraw.window = event->data.openblock.window;
  Wimp_RedrawWindow(&redraw, &more);
  origin_x = redraw.rect.min.x - redraw.scroll.x;
  origin_y = redraw.rect.max.y - redraw.scroll.y;

  while (more)
  {
    height = redraw.cliprect.max.y - redraw.cliprect.min.y;
    xstart = redraw.cliprect.min.x - height;
    xcorr = (xstart - origin_x) % hatch_size;
    ycorr = (origin_y - redraw.cliprect.min.y) % hatch_size;
    for (x = xstart-xcorr-ycorr; x < redraw.cliprect.max.x; x += hatch_size) {
      GFX_Move(x, redraw.cliprect.min.y);
      GFX_DrawBy(height, height);
    }
    ycorr = (origin_y - redraw.cliprect.max.y) % hatch_size;
    for (x = xstart-xcorr+ycorr-hatch_size; x < redraw.cliprect.max.x; x += hatch_size) {
      GFX_Move(x, redraw.cliprect.max.y-1);
      GFX_DrawBy(height, -height);
    }

    Wimp_GetRectangle(&redraw, &more);
  }

  return(TRUE);
}
