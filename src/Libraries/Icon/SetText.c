/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Icon.SetText.c
    Author:  Copyright © 1992, 1993, 1994 Jason Williams
    Version: 1.01 (12 Mar 1994)
    Purpose: 'Print' a string into an icon's text buffer.
*/

#include <string.h>

#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Coord.h"
#include "DeskLib:Icon.h"



extern void Icon_SetText(window_handle w, icon_handle i, const char *text)
/*
 * Copies the text string into the icon's indirected string buffer (and redraws)
 * If unable to set the text (incorrect icon type), it returns quietly
 * If text passed in is a NULL pointer, sets icon text to " "
 */
{
  icon_block  istate;
  char        temp[2] = " ";
  caret_block caret;
  int         len;

  if (text == NULL)
    text = temp;

  Wimp_GetIconState(w, i, &istate);
  if (istate.flags.value & (icon_TEXT | icon_INDIRECTED))
  {
    /* Indirected text icon, so set text field - ensure no buffer overflow */
    strncpy(istate.data.indirecttext.buffer, text,
              istate.data.indirecttext.bufflen - 1);
    istate.data.indirecttext.buffer[istate.data.indirecttext.bufflen-1] = 0;

    /* Ensure caret isn't beyond end of text */
    Wimp_GetCaretPosition( &caret );
    if ( caret.window == w && caret.icon == i )
    {
      len = strlen( istate.data.indirecttext.buffer );
      if ( caret.index > len )
      {
        caret.index = len;
        Wimp_SetCaretPosition( &caret );
      }
    }

    Wimp_SetIconState(w, i, 0, 0);
  }
}
