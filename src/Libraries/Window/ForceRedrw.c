/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Window.ForceRedrw.c
    Author:  Copyright © 1994 Lenny
    Version: 0.01 (18 Nov 1994)
    Purpose: Marks a screen area for redraw by the Wimp.
    History: 0.01 (18 Nov 94) : Added 'Window_ForceRedraw()'

*/

/* --- LOAD HEADERS ------------------------------------------------------ */

/* --- DeskLib ----------------------------------------------------------- */
#include "DeskLib:WimpSWIs.h"          /* Low-level WIMP commands          */
#include "DeskLib:Window.h"

/* === FUNCTION DEFINITIONS ============================================== */

extern void Window_ForceRedraw(window_handle wh, int x0, int y0, int x1, int y1)
/*
 *  Marks a screen area for redraw by the Wimp.
 *  (x0,y0) is the bottom left corner.
 *  Work Area coordinates, ie the top-left corner is generally 0,0 with y
 *  increasingly negative working down the screen.
 *
 *  Essentially just a veneer for Wimp_ForceRedraw().
 */
{
  window_redrawblock    block;

  block.window = wh;
  block.rect.min.x = x0;  block.rect.min.y = y0;
  block.rect.max.x = x1;  block.rect.max.y = y1;
  /******  FIND EQUIVALENT OF 'wimpt_noerr()'  ******/
  Wimp_ForceRedraw(&block);

}

/***************************************************************************/
