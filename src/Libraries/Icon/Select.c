#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"


extern void Icon_Select(window_handle window, icon_handle icon)
/* If the given icon is currently deselected, it is selected */
{
  icon_block istate;

  Wimp_GetIconState(window, icon, &istate);
  if (!istate.flags.data.selected)
    Wimp_SetIconState(window, icon, icon_SELECTED, icon_SELECTED);
}
