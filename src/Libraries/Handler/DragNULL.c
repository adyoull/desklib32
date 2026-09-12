#include "DeskLib:Wimp.h"
#include "DeskLib:Drag.h"
#include "DeskLib:Handler.h"

/*  Attach this handler to NULL events while dragging, after registering your
 *  handlers with Drag_SetHandlers (See drag.h)
 *  Note that this handler is of little use without also attaching the
 *  DragFinish handler as well.
 *  If you're using the event_ sublibrary, then use Drag_Initialise to attach
 *  these handlers for you.
 */

extern BOOL Handler_DragNULL(event_pollblock *event, void *reference)
{
  UNUSED ( reference);
  UNUSED ( event);

  if (drag_currentupdate != NULL)
    drag_currentupdate(drag_currentuserdata);

  return(FALSE);
}
