/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Template.LinkSprite.c
    Author:  Copyright © 1994 Lenny
    Version: 0.01 (18 Nov 1994)
    Purpose: Updates the cached Template data for the specified window,
             so that any sprite icons subsequently displayed in that window
             will be searched for in the specified sprite area.
    History: 0.01 (18 Nov 94) : Added 'Template_LinkSpriteArea()'

*/

/* --- LOAD HEADERS ------------------------------------------------------ */

/* --- DeskLib ----------------------------------------------------------- */
#include "DeskLib:Wimp.h"
#include "DeskLib:Sprite.h"            /* Sprite handling                  */
#include "DeskLib:Template.h"          /* Template loading and caching     */

/* === FUNCTION DEFINITIONS ============================================== */

extern void Template_LinkSpriteArea(const char *identifier, sprite_area area)
{
  window_block          *wblock;

  wblock = Template_Find(identifier);
  if (wblock != NULL)
    wblock->spritearea = area;
}

/***************************************************************************/
