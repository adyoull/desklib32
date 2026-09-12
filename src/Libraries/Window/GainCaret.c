/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Window.GainCaret.c
    Author:  Copyright © 1995 Cy Booker
    Version: 1.01 (21 Jun 1995)
    Purpose: High-level window management functions
    Mods:    Julian Smith - Modified to cope with Icon_SetCaret
                            which doesn't return an (os_error *)
                            yet.
*/

#include <stdlib.h>     /* malloc */

#include "DeskLib:Window.h"
#include "DeskLib:Icon.h"
#include "DeskLib:WimpSWIs.h"


extern os_error *Window_GainCaret(window_handle window) {
  os_error      *error;
  caret_block   caretblock;
  window_info   windowinfo;

  if (!window) {
    /* disown caret
     */
    caretblock.window = -1;
    caretblock.icon   = -1;
    caretblock.height = 0;
    caretblock.index  = -1;
    return Wimp_SetCaretPosition(&caretblock);
  }
  error = Wimp_GetCaretPosition(&caretblock);
  if (error) {
    return error;
  }
  if ((caretblock.window == window) && (caretblock.icon != -1)) {
    /* already in the window, in an icon, so abort now
     */
    return NULL;
  }
  error = Window_GetInfo3(window, &windowinfo);
  if (error) {
    return error;
  }
  if (windowinfo.block.numicons > 0) {
    icon_handle   *buffer;

    buffer = malloc((1+windowinfo.block.numicons)*sizeof(*buffer));
    if (buffer) {
      int               mask, settings;
      icon_handle       icon;

      mask = icon_TEXT |
             (icon_BUTTONTYPE * 0x0f) |
             icon_SHADED |
             icon_DELETED;
      settings = icon_TEXT |
                 (icon_BUTTONTYPE * iconbtype_WRITECLICKDRAG);  /* note! */
      error = Wimp_WhichIcon(window, buffer, mask, settings);
      icon = buffer[0];
      free(buffer);
      if (error) {
        return error;
      }
      if (icon != -1) {
        /*return Icon_SetCaret(window, icon);*/
        Icon_SetCaret(window, icon);
        return NULL;
        /* JS - Icon_SetCaret doesn't return an (os_error *) yet...	*/
      }
    }
  }
  caretblock.window   = window;
  caretblock.icon     = -1;
  caretblock.offset.x = 0;
  caretblock.offset.y = 0;
  caretblock.height   = 1 << 25;                /* caret is invisible */
  caretblock.index    = -1;
  return Wimp_SetCaretPosition(&caretblock);
}
