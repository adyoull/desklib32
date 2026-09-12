#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"

extern void Icon_Deselect(window_handle window, icon_handle icon)
/* If the given icon is currently selected, it is deselected */
{
  icon_block istate;

  Wimp_GetIconState(window, icon, &istate);
  if (istate.flags.data.selected)
    Wimp_SetIconState(window, icon, 0, icon_SELECTED);
}
