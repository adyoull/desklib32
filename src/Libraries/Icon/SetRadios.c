#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"

extern void Icon_SetRadios(window_handle window,
                           icon_handle first, icon_handle last,
                           icon_handle onradio)
/*
 * Pass in a group of icons (specified by lowest-numbered icon and
 * highest-numbered icon), and the icon from this group that you want
 * selected. All icons except the one you want selected will be deselected.
 */
{
  while (first <= last)
    Icon_Deselect(window, first++);       /* Turn off all between first,last */
  Icon_Select(window, onradio);           /* Turn on the requested one       */
}
