/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Window.SetExtent.c
    Author:  Copyright © 1994 Lenny
    Version: 0.01 (19 Nov 1994)
    Purpose: Change the size of a window's work area extent.
    History: 0.01 (19 Nov 94) : Added 'Window_SetExtent()'

*/

/* --- LOAD HEADERS ------------------------------------------------------ */

/* --- DeskLib ----------------------------------------------------------- */
#include "DeskLib:WimpSWIs.h"          /* Low-level WIMP commands          */
#include "DeskLib:Window.h"

/* === FUNCTION DEFINITIONS ============================================== */

extern void Window_SetExtent(window_handle wh, int x0, int y0, int x1, int y1)
/*
 *  Change the size of a window's work area extent.
 *
 *  When increasing the extent, the visible window size is not changed,
 *  though when decreasing the extent, the window will be shrunk in
 *  size, if necessary.
 */
{
  wimp_box              newextent;
  window_state          state;

  newextent.min.x = x0;  newextent.min.y = y0;
  newextent.max.x = x1;  newextent.max.y = y1;
  Wimp_SetExtent(wh, &newextent);
  Wimp_GetWindowState(wh, &state);
  if (state.flags.data.open) {  /* ie if window is open */
    Wimp_OpenWindow(&state.openblock);
  }

}

/***************************************************************************/
