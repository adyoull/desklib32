#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"

extern void Icon_ForceWindowRedraw(window_handle window, icon_handle icon)
/*  Force Redraws the area of WINDOW surrounding the icon, *including*
 *  the 3-d border (validation string "b<bordertype>")
 *  This is a bit obsolete now, but is still included as it may be useful
 *  code to look at if you want to do similar stuff...
 */
{
  window_redrawblock r;
  icon_block         istate;

  Wimp_GetIconState(window, icon, &istate);
  r.window    = window;
  r.rect.min.x = istate.workarearect.min.x - 4;
  r.rect.max.x = istate.workarearect.max.x + 4;
  r.rect.min.y = istate.workarearect.min.y - 4;
  r.rect.max.y = istate.workarearect.max.y + 4;
  Wimp_ForceRedraw(&r);
}
