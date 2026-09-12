#include "DeskLib:Wimp.h"
#include "DeskLib:Icon.h"
#include "DeskLib:Handler.h"


extern BOOL Handler_ClickOK(event_pollblock *event, void *reference)
{
  UNUSED( reference);

  if (!event->data.mouse.button.data.menu)               /* Not a MENU click */
  {
    Icon_Select(event->data.mouse.window, event->data.mouse.icon);
    Icon_ClickWait(33);
    Icon_Deselect(event->data.mouse.window, event->data.mouse.icon);
    return(TRUE);
  }

  return(FALSE);
}
