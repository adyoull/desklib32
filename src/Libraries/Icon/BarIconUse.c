/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Icon.BarIconUse.c
    Author:  Copyright © 1994 Peter Gaunt
    Version: 1.00 (12 Mar 1994)
             1.01 (05 Sep 2007) Use strncpy rather than strcpy
    Purpose: Place icon on the Icon Bar, using a caller-specified sprite
      	     area.
*/

#include <string.h>
#include <stdlib.h>

#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Error.h"
#include "DeskLib:Icon.h"

#define BARICON_XSIZE 68
#define BARICON_YSIZE 68


/*
 * Plonk a sprite from a USER sprite area on the icon bar
 * Returns icon handle.
 */

extern icon_handle Icon_BarIconUser(const char *spritename, window_handle pos,
      	      	      	      	    void *area)
{
  icon_createblock icreate;
  icon_handle      icon;
  char             *iconname;

  icreate.window = pos;
  icreate.icondata.flags.value = 0x1700310A;    /* Click-able sprite icon */

  iconname = malloc( wimp_MAXNAME + 1 );
  if (iconname == NULL)
    Error_Report(1,"Not enough memory");

  strncpy(iconname,spritename,wimp_MAXNAME);
  iconname[wimp_MAXNAME] = '\0';

  icreate.icondata.data.indirectsprite.name = iconname;
  icreate.icondata.data.indirectsprite.spritearea = area;
  icreate.icondata.data.indirectsprite.nameisname = wimp_MAXNAME;

  icreate.icondata.workarearect.min.x =
    icreate.icondata.workarearect.min.y = 0;

  icreate.icondata.workarearect.max.x =  BARICON_XSIZE;

  icreate.icondata.workarearect.max.y = BARICON_YSIZE;

  Wimp_CreateIcon(&icreate, &icon);
  return(icon);
}

