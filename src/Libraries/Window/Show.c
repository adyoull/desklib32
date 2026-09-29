/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Window.Show.c
    Author:  Copyright © 1992 Jason Williams
    Version: 1.00 (19 Mar 1992)
             1.01 (31 Aug 2007) Ensure window doesn't appear off-screen &
                                bring to front if already open
             1.02 (7  Jan 2008) Remove redundant code - OpenWindow only opens windows fully on screen anyhow
    Purpose: High-level window management functions: Show a window
*/


#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Window.h"
#include "DeskLib:Screen.h"

static wimp_point lastopenpos = {-1, -1};


extern void Window_Show(window_handle window, window_openpos openpos)
{
  window_state  wstate;
  wimp_point    moveto = {0, 0};
  int           w, h;

  Screen_CacheModeInfo();            /* Ensure got correct screen mode info. */

  Wimp_GetWindowState(window, &wstate);
  wstate.openblock.behind = -1;                             /* open in front */

  w = wstate.openblock.screenrect.max.x - wstate.openblock.screenrect.min.x;
  h = wstate.openblock.screenrect.max.y - wstate.openblock.screenrect.min.y;

  switch(openpos)
  {
    case open_CENTERED:
      moveto.x = (screen_size.x - w) / 2;
      moveto.y = (screen_size.y + h) / 2;
      break;

    case open_CENTEREDUNDERPOINTER:
      {
        mouse_block ptr;

        Wimp_GetPointerInfo(&ptr);
        moveto.x = ptr.pos.x - (w / 2);
        moveto.y = ptr.pos.y + (h / 2);
      }
      break;

    case open_OVERCARET:
      {
        caret_block  caret;
        window_state car_wstate;

        Wimp_GetCaretPosition(&caret);

        if (caret.window > 0)
        {
          Wimp_GetWindowState(caret.window, &car_wstate);

          moveto.x = car_wstate.openblock.screenrect.min.x +
                     (caret.offset.x - car_wstate.openblock.scroll.x) - 64;
          moveto.y = car_wstate.openblock.screenrect.max.y -
                     (caret.offset.y - car_wstate.openblock.scroll.y) + 64;
        }
        else
        {
          /* No caret, so just open centered on screen */
          moveto.x = (screen_size.x - w) / 2;
          moveto.y = (screen_size.y + h) / 2;
        }
      }
      break;

    case open_UNDERPOINTER:
      {
        mouse_block ptr;

        Wimp_GetPointerInfo(&ptr);
        moveto.x = ptr.pos.x - 64;
        moveto.y = ptr.pos.y + 64;
      }
      break;

    case open_NEARLAST:
      if (lastopenpos.x >= 0)
      {
        moveto.x = lastopenpos.x + 16;
        moveto.y = lastopenpos.y - 16;
      }
      else
      {
        moveto.x = (screen_size.x - w) / 2;
        moveto.y = (screen_size.y + h) / 2;
      }

      if (moveto.x > ((screen_size.x / 2) + 128))
        moveto.x = (screen_size.x / 2) - 128;

      if (moveto.y < ((screen_size.y / 2) - 128))
        moveto.y = (screen_size.y / 2) + 128;
      break;

    default:
      /* Open wherever it is defined in the template file, or where last open on screen. */
      moveto.x = wstate.openblock.screenrect.min.x;
      moveto.y = wstate.openblock.screenrect.max.y;
      break;
  }

  wstate.openblock.screenrect.min.x = moveto.x;
  wstate.openblock.screenrect.max.y = moveto.y;

  wstate.openblock.screenrect.max.x = wstate.openblock.screenrect.min.x + w;
  wstate.openblock.screenrect.min.y = wstate.openblock.screenrect.max.y - h;

  /* save last open position (for potential use next time with open_NEARLAST) */
  lastopenpos.x = wstate.openblock.screenrect.min.x;
  lastopenpos.y = wstate.openblock.screenrect.max.y;

  /* If the window's already open, bring it to the top of thw window stack, otherwise open it */
  if (wstate.flags.data.open)
    Window_BringToFront(window);
  else
    Wimp_OpenWindow(&wstate.openblock);
}
