/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Icon.RadioInEsg.c
    Author:  Copyright © 1994 Lenny
    Version: 0.01 (19 Nov 1994)
    Purpose: Returns the icon_handle (within a specified ESG) which is SET.
             Similar to Icon_WhichRadio, except the icons considered do
             not need to be consecutively numbered.
    History: 0.01 (19 Nov 94) : Added 'Icon_WhichRadioInEsg()'

*/

/* --- LOAD HEADERS ------------------------------------------------------ */

/* --- DeskLib ----------------------------------------------------------- */
#include "DeskLib:WimpSWIs.h"          /* Low-level WIMP commands          */
#include "DeskLib:Icon.h"

/* === FUNCTION DEFINITIONS ============================================== */

extern int Icon_WhichRadioInEsg(window_handle wh, int esg)
{
  icon_handle           ih[2];
  int                   mask, set;

  mask = 0x2F0000;  /* 'Selected' and 'ESG' fields of icondata */
  set  = 0x200000 | (esg << 16);

  Wimp_WhichIcon(wh, ih, mask, set);

  return (int) (ih[0]);

}

/***************************************************************************/
