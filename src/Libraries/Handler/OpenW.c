#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Handler.h"


extern BOOL Handler_OpenWindow(event_pollblock *event, void *reference)
{
  UNUSED( reference);
  Wimp_OpenWindow(&(event->data.openblock));
  return(TRUE);
}
