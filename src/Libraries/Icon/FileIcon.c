/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Icon.FileIcon.c
    Author:  Copyright © 1994 Tim Browse
    Version: 1.01 (26 Jun 1994)
             1.02 (05 Sep 2007) Changed sprintf to snprintf
    Purpose: Changes an icon in a window to display the correct filetype
      	     sprite for the given filetype number.
*/


#include <stdio.h>

#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"

icon_handle Icon_FileIcon(window_handle window, icon_handle icon, int filetype)
{
  /* Icon should be an indirected text-only icon with enough room in text
     buffer to hold the sprite name.
     This converts to an indirected sprite-only icon, and fills in the sprite
     name and area.
  */
  icon_createblock iconcreate;
  icon_handle handle;

  /* Get the data for this icon */
  Wimp_GetIconState(window, icon, &iconcreate.icondata);

  /* Delete this icon - we have to change the data fields */
  Wimp_DeleteIcon(window, icon);

  /* Put sprite name in name field */
  snprintf((char *)(int)iconcreate.icondata.data.indirectsprite.name,
           iconcreate.icondata.data.indirectsprite.nameisname,
           "file_%03x", filetype);

  /* Fill in sprite area */
  iconcreate.icondata.data.indirectsprite.spritearea = (unsigned int *) 1;

  /* Fill in sprite name length */
  iconcreate.icondata.data.indirectsprite.nameisname = 8; /* file_xxx */

  /* Change to sprite-only */
  iconcreate.icondata.flags.data.text   = FALSE;
  iconcreate.icondata.flags.data.sprite = TRUE;

  /* Re-create the icon */
  iconcreate.window = window;
  Wimp_CreateIcon(&iconcreate, &handle);

  /* Force the icon to be redrawn */
  Wimp_SetIconState(window, handle, 0, 0);

  /* Return the new icon handle (in case it has changed) */
  return handle;
}
