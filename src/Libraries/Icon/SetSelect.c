#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"


extern void Icon_SetSelect(window_handle window, icon_handle icon, int flag)
/* If flag == 0, (and the icon is currently selected) it will be deselected
 * If flag == 1, (and the icon is currently not selected) it will be selected
 */
{
  icon_block istate;

  Wimp_GetIconState(window, icon, &istate);
  if (flag)
  {
    if (!istate.flags.data.selected)
      Wimp_SetIconState(window, icon, icon_SELECTED, icon_SELECTED);
  }
  else
  {
    if (istate.flags.data.selected)
      Wimp_SetIconState(window, icon, 0, icon_SELECTED);
  }
}
