#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Handler.h"

extern BOOL Handler_Key(event_pollblock *event, void *reference)
{
  UNUSED( reference);
  Wimp_ProcessKey(event->data.key.code);
  return(TRUE);
}
