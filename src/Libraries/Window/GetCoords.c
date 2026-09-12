/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Window.GetCoords.c
    Author:  Copyright © 1994 Lenny
    Version: 0.01 (20 Nov 1994)
    Purpose: Fill a 'convert_block' structure with the relevant info for
             the specified window.
    History: 0.01 (20 Nov 94) : Added 'Window_GetCoords()'

*/

/* --- LOAD HEADERS ------------------------------------------------------ */

/* --- DeskLib ----------------------------------------------------------- */
#include "DeskLib:WimpSWIs.h"          /* Low-level WIMP commands          */

#include "DeskLib:Coord.h"             /* OS - workarea coord conversion   */
#include "DeskLib:Window.h"

/* === FUNCTION DEFINITIONS ============================================== */

/* ----------------------------------------------------------------------- */

extern void Window_GetCoords(window_handle wh, convert_block *coords)
/*
 *  The coords 'convert_block' passed into this function is filled
 *  with the relevant info for window 'wh'.
 */
{
    window_state    wstate;

    Wimp_GetWindowState(wh, &wstate);
    coords->screenrect.min.x = wstate.openblock.screenrect.min.x;
    coords->screenrect.min.y = wstate.openblock.screenrect.min.y;
    coords->screenrect.max.x = wstate.openblock.screenrect.max.x;
    coords->screenrect.max.y = wstate.openblock.screenrect.max.y;
    coords->scroll.x = wstate.openblock.scroll.x;
    coords->scroll.y = wstate.openblock.scroll.y;
    /*  OR (?)  */
/*    memcpy(coords, &wstate.openblock, sizeof(convert_block));  */

}

/***************************************************************************/
