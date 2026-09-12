#include "DeskLib:Wimp.h"
#include "DeskLib:Drag.h"
#include "DeskLib:Handler.h"

/*  Attach this handler to Drag finish events while dragging, after
 *  registering your handlers with Drag_SetHandlers (See drag.h)
 *  You will probably want to attach the DragNULL handler as well.
 *  If you're using the event_ sublibrary, then use Drag_Initialise to attach
 *  these handlers for you.
 */

extern BOOL Handler_DragFinish(event_pollblock *event, void *reference)
{
  UNUSED( reference);
  UNUSED( event);

  if (drag_currentcomplete != NULL)
    drag_currentcomplete(drag_currentuserdata);

  drag_currentuserdata = NULL;  /* Disable handlers again */
  drag_currentupdate   = NULL;
  drag_currentcomplete = NULL;

  return(FALSE);
}
