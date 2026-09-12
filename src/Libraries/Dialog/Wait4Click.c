/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Dialog.Wait4Click.c
    Author:  Copyright © 1993 Tim Browse and Jason Williams
    Version: 1.00 (10 Jul 1993)
    Purpose: Very high level window (dialogue) handling -
             Processing events for dialogue windows
*/


#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"

#include "DeskLib:Dialog.h"
#include "DeskLib:Event.h"


extern icon_handle Dialog_WaitForClick(dialog dbox)
/*  Wait for choice to be made by the user, or for the window to close.
 *  If user closes window by clicking outside, dialog_CLOSE is returned, else
 *  the icon which was clicked is returned.
 *  This function may be called repeatedly while you wait for a particular
 *  icon to be clicked.
 *
 *  Some care is taken to ensure that the 'dbox->lastchoice' value will
 *  always be a valid icon handle except when no icns have yet been clicked.
 *  (i.e. if you click icon 3, then close the window, lastchoice will be 3
 *  while this function will return dialog_CLOSE)
 */
{
  window_state wstate;
  icon_handle  choice, lastchoice;

  lastchoice = dbox->lastclicked;
  dbox->lastclicked = choice = dialog_NOCHOICE;

  do
  {
    Event_Poll();

    /*  If user has clicked outside window, hit return, or clicked the close
     *  box, causing it to close, we take that as a 'Cancel': dialog_CLOSE.
     */
    Wimp_GetWindowState(dbox->window, &wstate);
    if (wstate.flags.data.open)                /* Window has not been closed */
      choice = dbox->lastclicked;
    else
    {
      choice = dialog_CLOSE;
      dbox->state.persist = FALSE;
      dbox->state.stillopen =FALSE;
    }

  } while (choice == dialog_NOCHOICE);    /* Repeat until close or click     */

  if (dbox->lastclicked < 0)              /* Ensure return last clicked icon */
    dbox->lastclicked = lastchoice;       /* when window is closed etc       */

  return(choice);
}

