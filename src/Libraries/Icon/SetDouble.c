#include <stdio.h>
#include <string.h>

#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"


extern void Icon_SetDouble(window_handle w, icon_handle i,
                           double value, int decimalplaces)
/*
 * Sets the given icon's text to hold the number in "value". (and redraws icon)
 * After the decimal place, up to "decimalplaces" digits will be printed
 * If the number is too big (too many digits), it will be truncated to fit
 * (which may completely destroy the value you wished to represent, or
 * just reduce its accuracy)
 * If unable to set the text (incorrect icon type), it returns quietly
 */
{
  char       text[32], format[16];

  snprintf(format, sizeof(format), "%%.%df", decimalplaces);
  snprintf(text, sizeof(text), format, value);

  Icon_SetText(w, i, text);
}
