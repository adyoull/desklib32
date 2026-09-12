#include "Desklib:Wimp.h"
#include "Desklib:Screen.h"
#include "Desklib:Window.h"
#include "DeskLib:Handler.h"


extern BOOL Handler_ModeChange(event_pollblock *event, void *reference)
{
  wimp_point old_eig = screen_eig;

  UNUSED( reference);
  UNUSED( event);

  Screen_CacheModeInfo();

  /*  If the screen resolution in pixels/inch has changed, then re-get all
   *  of the outline fonts used in windows.
   *  (This is necessary when changing between some screen resolutions, e.g.
   *  mode 12 <-> 9, 12 <-> 20, etc)
   */
  if (old_eig.y != screen_eig.y || old_eig.x != screen_eig.x)
    Window_ModeChange();

  return(FALSE);
}

