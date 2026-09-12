/* Icon_SetTextRJ
 * (c) Copyright 1993 Erik do Kort
 */

#include <string.h>

#include "DeskLib:Icon.h"
#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"


extern void Icon_SetTextRJ(window_handle w, icon_handle i, const char *text)
/*
 * Copies the text string into the icon's indirected string buffer (and redraws)
 * This text is "right justified", meaning that if the text doesn't fit,
 * it is truncated at the *LEFT*-hand side, and preceded by a '...' char
 * If unable to set the text (incorrect icon type), it returns quietly
 * If text passed in is a NULL pointer, sets icon text to '\0'
 */
{
  icon_block  istate;
  int         empty = 0;
  caret_block caret;
  int         len;

  if (text == NULL)
    text = (char *)&empty;

  Wimp_GetIconState(w, i, &istate);
  if (istate.flags.value & (icon_TEXT | icon_INDIRECTED))
  {
    /* Indirected text icon, so set text field - ensure no buffer overflow
     */
    register int  textlength = strlen(text) + 1 ;
    register int  bufflength = istate.data.indirecttext.bufflen ;
    register char *buffer    = istate.data.indirecttext.buffer ;
    if ( bufflength < textlength )
    {
      text += textlength - bufflength ;
      if ( bufflength > 2 )
      {
        *buffer++ = '\x8c' ;
        text += 1 ;
      }
    }
    strcpy(buffer, text) ;

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
