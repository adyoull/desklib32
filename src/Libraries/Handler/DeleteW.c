#include "DeskLib:Wimp.h"
#include "DeskLib:Window.h"
#include "DeskLib:Handler.h"


extern BOOL Handler_DeleteWindow(event_pollblock *event, void *reference)
/*  Alternate handler to Handler_Close, which calls the higher-level
 *  Window_Delete() to close and delete the window, release memory, and
 *  remove all handlers attached to the window.
 */
{
  UNUSED( reference);

  Window_Delete(event->data.openblock.window);
  return(TRUE);
}
