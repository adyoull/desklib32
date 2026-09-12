#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"

extern void Icon_LoseCaret(window_handle window, icon_handle icon)
/* Icon_LoseCaret
 * This routine ensures that the caret is *not* contained within the
 * designated icon. Only sets a new position if the icon currently contains
 *  the caret.
 */
{
  caret_block caret;

  Wimp_GetCaretPosition(&caret);
  if (caret.window == window && caret.icon == icon)
    Icon_SetCaret(-1, -1);
}
