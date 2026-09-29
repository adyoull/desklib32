/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Pane.c
    Author:  Copyright © 1994 Ainsley Pereira. Major code rewrite by Keith Hall
    Version: 1.11 (30th March 94)
    Purpose: Handles windows with panes.
*/

#include <stdlib.h>

#include "DeskLib:Pane.h"

#include "PaneDefs.h"


extern pane_data *Pane_GetSysHandle(window_handle master)
{
  curpane = pane_root;

  while(curpane)
  {
    if(curpane->panedata.master == master)
      return &curpane->panedata;
    curpane = curpane->next;
  }
  return NULL;
}


extern int Pane_SetFlags(window_handle master, int flags)
{
  curpane = pane_root;

  while(curpane)
  {
    if(curpane->panedata.master == master)
    {
      if(flags>=0)
        curpane->panedata.flags.value=flags;
      return curpane->panedata.flags.value;
    }
    curpane = curpane->next;
  }
  return 0;
}


extern void Pane_Delete(window_handle window)
{
  pane_link *lastpane=curpane=pane_root;

  while(curpane)
  {
    if(curpane->panedata.master == window)
    {
      Window_Delete(curpane->panedata.master);
      Window_Delete(curpane->panedata.pane);
      lastpane->next=curpane->next;
      free(curpane);
      if(lastpane->next==NULL)
        break;
      curpane=lastpane->next;
    }
    lastpane=curpane;
    curpane = curpane->next;
  }
}
