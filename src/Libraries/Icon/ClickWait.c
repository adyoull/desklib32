#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Time.h"
#include "DeskLib:Event.h"
#include "DeskLib:Icon.h"


extern void Icon_ClickWait(int waittime)
{
  static int      waitingonclick = FALSE;
  int             time;
  event_pollblock event;
  event_pollmask  mask;

  if (waitingonclick)        /* Don't allow re-entrant call on this function */
    return;

  waitingonclick = TRUE;
  time = Time_Monotonic() + waittime;

  do
  {
    mask.value = 0;                           /* enable null polls */
    Wimp_PollIdle(mask, &event, time);        /* Wait until time up or event */

    if (event.type != event_CLICK && event.type != event_NULL)
      Event_Process(&event);                           /* Process this event */

  } while (event.type == event_CLICK || event.type == event_REDRAW);

  /*  Wait until not a button click event, and also ignore redraws as a
   *  termination condition, as redraw is very likely (indenting a button)
   *  on the first poll after the click which called us!
   */

  waitingonclick = FALSE;
}
