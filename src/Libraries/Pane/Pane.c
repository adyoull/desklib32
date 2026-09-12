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

pane_link *pane_root=NULL;
pane_link *curpane=NULL;


extern BOOL Pane_OpenEventHandler(event_pollblock *event,
                                  void *reference)
{
  window_state pstate;

  UNUSED( reference);

  curpane=pane_root;

  while(curpane)
  {
    if(event->data.openblock.window == curpane->panedata.master)
    {
      Wimp_GetWindowState(curpane->panedata.pane, &pstate);

      if(curpane->panedata.flags.data.fixed)
      {
        pstate.openblock.screenrect.max.y = event->data.openblock.screenrect.max.y - curpane->panedata.offset.y;
        pstate.openblock.screenrect.min.y = pstate.openblock.screenrect.max.y - curpane->panedata.size.y;
      }
      else
      {
        pstate.openblock.screenrect.min.y = event->data.openblock.screenrect.min.y +curpane->panedata.offset.y +42;
        pstate.openblock.screenrect.max.y = pstate.openblock.screenrect.min.y + curpane->panedata.size.y;
      }
      pstate.openblock.screenrect.min.x = event->data.openblock.screenrect.min.x + curpane->panedata.offset.x;

      if(curpane->panedata.flags.data.resize)
      {
        if(event->data.openblock.screenrect.max.x > pstate.openblock.screenrect.min.x + curpane->panedata.size.x)
          pstate.openblock.screenrect.max.x = pstate.openblock.screenrect.min.x + curpane->panedata.size.x;
        else
          pstate.openblock.screenrect.max.x = event->data.openblock.screenrect.max.x;
      }
      else
        pstate.openblock.screenrect.max.x = pstate.openblock.screenrect.min.x + curpane->panedata.size.x;
      pstate.openblock.behind = event->data.openblock.behind;
      Wimp_OpenWindow(&pstate.openblock);
      event->data.openblock.behind = curpane->panedata.pane;
      Wimp_OpenWindow(&event->data.openblock);
      return (TRUE);
    }
    curpane=curpane->next;
  }
  return (FALSE);
}

extern void Pane_Link(window_handle mast,
                      window_handle pane,
                      wimp_point   *offset,
                      wimp_point   *size,
                      int           flags)
{
  window_state maststate, panestate;

  Wimp_GetWindowState(mast, &maststate);
  Wimp_GetWindowState(pane, &panestate);

  if(pane_root == NULL)
  {
    if((curpane = pane_root = calloc(1, sizeof(pane_link))) == NULL)
      return;       /* return quietly */
  }
  else
  {
    if((curpane->next = calloc(1, sizeof(pane_link))) == NULL)
      return;       /* return quietly */
    curpane = curpane->next;
  }
  curpane->next = NULL;

  curpane->panedata.master = mast;
  curpane->panedata.pane = pane;
  curpane->panedata.flags.value = flags;
  curpane->panedata.flags.data.isopen = 0;

  if(offset)
    curpane->panedata.offset = *offset;
  else
  {
    curpane->panedata.offset.x = panestate.openblock.screenrect.min.x - maststate.openblock.screenrect.min.x;
    if(curpane->panedata.flags.data.fixed)
      curpane->panedata.offset.y = maststate.openblock.screenrect.max.y - panestate.openblock.screenrect.max.y;
    else
      curpane->panedata.offset.y = panestate.openblock.screenrect.min.y - maststate.openblock.screenrect.min.y -42;
  }

  if(size)
    curpane->panedata.size = *size;
  else
  {
    curpane->panedata.size.x = panestate.openblock.screenrect.max.x - panestate.openblock.screenrect.min.x;
    curpane->panedata.size.y = panestate.openblock.screenrect.max.y - panestate.openblock.screenrect.min.y;
  }
}

extern window_handle Pane_CreateAndLink(const char  *mastname,
                                        const char  *panename,
                                        int         mastmaxsize,
                                        int         panemaxsize,
                                        wimp_point *offset,
                                        wimp_point *size,
                                        int         flags)
{
  window_handle mast, pane;

  mast=Window_Create(mastname, mastmaxsize);
  pane=Window_Create(panename, panemaxsize);

  Pane_Link(mast, pane, offset, size, flags);

  return mast;
}

extern void Pane_Show(window_handle  window,
                      window_openpos openpos)
{
  window_state pstate;
  window_state mstate;
  curpane = pane_root;

  while(curpane)
  {
    if(curpane->panedata.master == window)
    {
      if(curpane->panedata.flags.data.isopen == 0)
      {
        Window_Show(curpane->panedata.master, openpos);
        Wimp_GetWindowState(curpane->panedata.master, &mstate);
        Wimp_GetWindowState(curpane->panedata.pane, &pstate);

        if(curpane->panedata.flags.data.fixed)
        {
          pstate.openblock.screenrect.max.y = mstate.openblock.screenrect.max.y - curpane->panedata.offset.y;
          pstate.openblock.screenrect.min.y = pstate.openblock.screenrect.max.y - curpane->panedata.size.y;
        }
        else
        {
          pstate.openblock.screenrect.min.y = mstate.openblock.screenrect.min.y +curpane->panedata.offset.y +42;
          pstate.openblock.screenrect.max.y = pstate.openblock.screenrect.min.y + curpane->panedata.size.y;
        }
        pstate.openblock.screenrect.min.x = mstate.openblock.screenrect.min.x + curpane->panedata.offset.x;

        if(curpane->panedata.flags.data.resize)
        {
          if(mstate.openblock.screenrect.max.x > pstate.openblock.screenrect.min.x + curpane->panedata.size.x)
            pstate.openblock.screenrect.max.x = pstate.openblock.screenrect.min.x + curpane->panedata.size.x;
          else
            pstate.openblock.screenrect.max.x = mstate.openblock.screenrect.max.x;
        }
        else
          pstate.openblock.screenrect.max.x = pstate.openblock.screenrect.min.x + curpane->panedata.size.x;
        pstate.openblock.behind = mstate.openblock.behind;
        Wimp_OpenWindow(&pstate.openblock);
        mstate.openblock.behind = curpane->panedata.pane;
        Wimp_OpenWindow(&mstate.openblock);

        curpane->panedata.flags.data.isopen = 1;
      }
    }
    curpane = curpane->next;
  }
}

extern void Pane_Hide(window_handle window)
{
  curpane = pane_root;

  while(curpane)
  {
    if(curpane->panedata.master == window)
    {
      if(curpane->panedata.flags.data.isopen == 1)
      {
        Window_Hide(curpane->panedata.master);
        Window_Hide(curpane->panedata.pane);
        curpane->panedata.flags.data.isopen = 0;
      }
    }
    curpane = curpane->next;
  }
}
