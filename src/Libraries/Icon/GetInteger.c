#include <stdlib.h>

#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"



extern int Icon_GetInteger(window_handle w, icon_handle i)
/*
 * Gets the given icon's text and returns it in the form of an integer
 * numeric value. 0 will be returned from any error/invalid text
 */
{
  icon_block istate;

  Wimp_GetIconState(w, i, &istate);
  if (istate.flags.value & (icon_TEXT | icon_INDIRECTED))
    return(atoi(istate.data.indirecttext.buffer));

  return(0);
}
