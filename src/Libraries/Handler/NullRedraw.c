#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Handler.h"


extern BOOL Handler_NullRedraw(event_pollblock *event, void *reference)
{
  window_redrawblock redraw;
  BOOL more;

  UNUSED( reference);

  redraw.window = event->data.openblock.window;
  Wimp_RedrawWindow(&redraw, &more);

  while (more)
  {
    /* Insert your window redraw code here */
    /* e.g. Redraw_ThreeD(&redraw);        */
    Wimp_GetRectangle(&redraw, &more);
  }

  return(TRUE);
}
