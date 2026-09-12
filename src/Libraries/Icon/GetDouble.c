#include <stdlib.h>

#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"



extern double Icon_GetDouble(window_handle w, icon_handle i)
/*
 * Gets the given icon's text and returns it in the form of an integer
 * numeric value. 0 will be returned from any error/invalid text
 */
{
  icon_block istate;

  Wimp_GetIconState(w, i, &istate);
  if (istate.flags.value & (icon_TEXT | icon_INDIRECTED))
    return(atof(istate.data.indirecttext.buffer));

  return(0);
}
