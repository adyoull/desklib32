#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"


extern void Icon_SetDeleted(window_handle window, icon_handle icon, int flag)
/* If flag == 0, (and the icon is currently deleted) it will be undeleted
 * If flag == 1, (and the icon is currently not deleted) it will be deleted
 */
{
  icon_block istate;

  Wimp_GetIconState(window, icon, &istate);
  if (flag)
  {
    if (!istate.flags.data.deleted)
      Wimp_SetIconState(window, icon, icon_DELETED, icon_DELETED);
  }
  else
  {
    if (istate.flags.data.deleted)
      Wimp_SetIconState(window, icon, 0, icon_DELETED);
  }
}
