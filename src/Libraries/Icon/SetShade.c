#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"

extern void Icon_SetShade(window_handle window, icon_handle icon, int flag)
/* If flag == 0, (and the icon is currently shaded) it will be un-shaded
 * If flag == 1, (and the icon is currently not shaded) it will be shaded
 */
{
  icon_block istate;

  Wimp_GetIconState(window, icon, &istate);
  if (flag)
  {
    if (!istate.flags.data.shaded)
      Wimp_SetIconState(window, icon, icon_SHADED, icon_SHADED);

    Icon_LoseCaret(window, icon);    /* Lose the caret if it is in this icon */
  }
  else
  {
    if (istate.flags.data.shaded)
      Wimp_SetIconState(window, icon, 0, icon_SHADED);
  }
}
