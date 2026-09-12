#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"

extern void Icon_Unshade(window_handle window, icon_handle icon)
/* if the icon is currently shaded (inactive) it is unshaded (made active) */
{
  icon_block istate;

  Wimp_GetIconState(window, icon, &istate);
  if (istate.flags.data.shaded)
    Wimp_SetIconState(window, icon, 0, icon_SHADED);
}
