/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Dialog.CreatDstry.c
    Author:  Copyright © 1993 Tim Browse and Jason Williams
    Version: 1.01 (02 Mar 1993)
    Purpose: Very high level window (dialogue) handling -
             Creating and destroying dialogues
*/

#include <stdlib.h>

#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"

#include "DeskLib:Dialog.h"
#include "DeskLib:Event.h"
#include "DeskLib:Window.h"


static BOOL EventHandler(event_pollblock *event, void *reference)
{
  dialog_record *dbox = (dialog_record *) reference;

  switch (event->type)
  {
     case event_CLOSE:
      /*  User has clicked on close icon - We just close the window, as
       *  Dialog_WaitForClick will notice this and take appropriate action
       */
      Window_Hide(dbox->window);
      return(TRUE);

    case event_CLICK:
      /* Ignore work-area click events */
      if (event->data.mouse.icon >= 0)
      {
      	dbox->lastclicked   = event->data.mouse.icon;
      	dbox->button.value  = event->data.mouse.button.value;
      	dbox->state.persist = event->data.mouse.button.data.adjust;
      	return(TRUE);
      }
    default:
      break;
  }

  return(FALSE);          /* Allow other event handlers to handle this event */
}


extern dialog Dialog_Create(const char *template_name, int maxtitlesize)
/*  Returns a pointer to a dialog record, or NULL if it fails */
{
  window_handle window;
  dialog d;

  window = Window_Create(template_name, maxtitlesize);
  if (window == (window_handle)0) return(NULL);

  d = (dialog) malloc(sizeof(dialog_record));
  if (d == NULL) return(NULL);

  d->window         = window;
  d->lastclicked    = dialog_NOCHOICE;
  d->state.persist  = TRUE;
  d->state.isstatic = d->state.stillopen = FALSE;

  /* Attach the event handler */
  Event_Claim(event_NULL, event_ANY, event_ANY, EventHandler, d);
  Event_Claim(event_ANY, window, event_ANY, EventHandler, d);

  return(d);
}


extern void Dialog_Destroy(dialog dbox)
{
  if (dbox != NULL)
  {
    Event_Poll();     /* for RO3 bug */

    /* Remove event handler */
    Event_Release(event_NULL, event_ANY, event_ANY, EventHandler, dbox);
    Event_Release(event_ANY, dbox->window, event_ANY, EventHandler, dbox);

    Dialog_Hide(dbox);
    Window_Delete(dbox->window);
    free(dbox);
  }
}
