#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"


extern void Icon_SetCaret(window_handle window, icon_handle icon)
/*
 * This routine sets the caret within the requested icon. It places the
 * caret at the (righthand) end of the text in the icon. If the icon is not
 * a text icon, then the function returns quietly
 */
{
  caret_block caret;

  caret.window = window;
  caret.icon   = icon;

  if (window >= 0 && icon >= 0)
  {
    icon_block    istate;
    register char *buff;
    register int  index, maxlen;

    Wimp_GetIconState(window, icon, &istate);

    if (!istate.flags.data.text || istate.flags.data.shaded)
      return;          /* Not a text icon, or is shaded, so can't grab caret */

    if (istate.flags.data.indirected)
    {
      maxlen = istate.data.indirecttext.bufflen;
      buff   = istate.data.indirecttext.buffer;
    }
    else
    {
      maxlen = 12;
      buff = istate.data.text;
    }

    index = 0;                                 /* Place caret at end of text */
    while(index < maxlen && buff[index]>31)
      index++;
    caret.index = index;
  }
  else
    caret.index = 0;

  caret.offset.y = caret.offset.x = 0;
  caret.height = -1;
  Wimp_SetCaretPosition(&caret);
}
