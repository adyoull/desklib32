#include <stdio.h>
#include <string.h>

#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"


extern void Icon_SetInteger(window_handle w, icon_handle i, int value)
/*
 * Sets the given icon's text to hold the number in "value". (and redraws icon)
 * If unable to set the text (incorrect icon type), it returns quietly
 */
{
  char       text[16];

  snprintf(text, sizeof(text), "%d", value);
  Icon_SetText(w, i, text);
}
