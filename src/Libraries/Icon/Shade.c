#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"


extern void Icon_Shade(window_handle window, icon_handle icon)
/*
 * if the icon is currently unshaded (active) it is shaded (made inactive)
 * This includes removing the caret from the icon.
 */
{
  icon_block istate;

  Wimp_GetIconState(window, icon, &istate);
  if (!istate.flags.data.shaded)
    Wimp_SetIconState(window, icon, icon_SHADED, icon_SHADED);

  Icon_LoseCaret(window, icon);      /* Lose the caret if it is in this icon */
}
