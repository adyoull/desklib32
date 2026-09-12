#include "DeskLib:Wimp.h"
#include "DeskLib:Event.h"
#include "DeskLib:Window.h"


extern BOOL Window_HelpHandler(event_pollblock *event, void *reference);


/* NOTE:
 * By #defining "USE_EVENTMSG" on compilation, the HelpHandler will be
 * called ONLY for a HelpRequest message, but this will also pull in the
 * "EventMsg" code.
 *
 * This function is basically here to show you how to include the
 * Window_HelpHandler under your own steam, using either EventMsg or Event
 *
 * use: #define USE_EVENTMSG 1
 *
 */


extern BOOL Window_AutoHelp(window_handle window, icon_handle icon)
{
#ifdef USE_EVENTMSG
  return(EventMsg_Claim(message_HELPREQUEST, window,
                          Window_HelpHandler, NULL);
#else
  return(Event_Claim(event_USERMESSAGERECORDED, window, icon,
                                                 Window_HelpHandler, NULL));
#endif
}
