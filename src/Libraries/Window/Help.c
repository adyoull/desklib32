/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Window.Help.c
    Author:  Copyright © 1992 Jason Williams
    Version: 1.00 (09 Apr 1992)
             1.01 (05 Sep 2007) Change use of sprintf to snprintf
    Purpose: Adds an event handler that replies to HelpRequest messages
             by taking the window's template name and icon number, and
             looking up (e.g. "template.1") in the Msgs file.
*/

#include <stdio.h>
#include "string.h"

#include "DeskLib:Event.h"
#include "DeskLib:Window.h"
#include "DeskLib:Msgs.h"
#include "DeskLib:WimpSWIs.h"



extern BOOL Window_HelpHandler(event_pollblock *event, void *reference)
{
  char mtemplate[16], number[16];
  message_destinee replyto;

  UNUSED( reference);

  if (event->data.message.header.action == message_HELPREQUEST)
  {
    Window_ParentName(event->data.message.data.helprequest.where.window,
                      mtemplate);
    mtemplate[8] = '\0';         /* tags can't be more than 8 characters long */

    snprintf(number, 8, ".%d", event->data.message.data.helprequest.where.icon);
    strcat(mtemplate, number); /* 8 + 8 is max size so can't be larger than 16 */

    if (Msgs_Lookup(mtemplate, event->data.message.data.helpreply.text, 235))
    {
      event->data.message.header.yourref = event->data.message.header.myref;
      event->data.message.header.action = message_HELPREPLY;
      event->data.message.header.size = 256;
      replyto = event->data.message.header.sender;
      Wimp_SendMessage( event_SENDWANTACK, &event->data.message, replyto, 0);
      return(TRUE);  /* Only claim message event if successfully reply to it */
    }
  }
  return(FALSE);
}
