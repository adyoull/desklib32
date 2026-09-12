#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Window.h"
#include "DeskLib:Handler.h"

extern BOOL Handler_CloseWindow(event_pollblock *event, void *reference)
{
  UNUSED( reference);

  Wimp_CloseWindow(event->data.openblock.window);
  return(TRUE);
}
