/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Menu.PopUpAuto.c
    Author:  Copyright © 1994 Tony Houghton
    Version: 1.00 (25 Sep 1994)
    Purpose: Pop up a menu to the right of a button icon, finding the icon from
             event_lastevent
*/

#include "DeskLib:Menu.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Event.h"

void Menu_PopUpAuto(menu_ptr menu)
{
  if (event_lastevent.type != event_CLICK)
    return;
  if (event_lastevent.data.mouse.window == -1 ||
      event_lastevent.data.mouse.window == window_ICONBAR)
    return;
  if (event_lastevent.data.mouse.icon == -1)
    return;
  Menu_PopUp(menu, event_lastevent.data.mouse.window,
                   event_lastevent.data.mouse.icon);
}
