#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"


extern int Icon_WhichRadio(window_handle window,
                           icon_handle first, icon_handle last)
/*
 * This function accepts parameters for the first (lowest numbered) and
 * last (highest numnbered) in a group of icons (generally a group of radio
 * buttons). It returns the icon number of the first icon it finds within
 * this range which is selected (or "last" if none are selected).
 * Use it to find which of a group of radios is selected.
 */
{
  icon_block istate;

  while (first < last)
  {
    Wimp_GetIconState(window, first, &istate);
    if (istate.flags.data.selected)
      return(first);

    first++;
  }

  return(last);
}
