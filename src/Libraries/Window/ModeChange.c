/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Window.ModeChange.c
    Author:  Copyright © 1993 Tim Browse and Jason Williams
             (Original code by Tim, massive changes and rewriting by Jason ;-)
    Version: 1.02 (28 Jun 1993)
    Purpose: High-level window management functions:
             Fix windows to use new font handles after a mode change.
             (This even works with open windows, so long as they were created
             with Window_ calls)
*/


#include "Desklib:Error.h"
#include "Desklib:Event.h"
#include "Desklib:Font.h"
#include "Desklib:LinkList.h"
#include "Desklib:Screen.h"
#include "Desklib:Template.h"
#include "Desklib:Window.h"
#include "Desklib:WimpSWIs.h"

#include "WindowDefs.h"

#include <string.h>


extern linklist_header window_listanchor;


/* This constant is used to mask out all icon flag fields except the font handle */
#define FONT_MASK ((unsigned) 0xFF000000)


static void MapFontHandles(window_block  *wblock,
                           window_handle window,
                           font_array    *fontarray)
/*  This function goes through a window block, and changes each icon's font
 *  handle (if any) by looking up its current handle in the given fontarray,
 *  and using that entry in the fontarray as the new font handle to use
 */
{
  icon_block    *icons;
  unsigned int   i;

  icons = (icon_block *) (wblock + 1);    /* pointer arithmetic! */
  for (i = 0; i < wblock->numicons; i++)
  {
    if (icons[i].flags.data.font)
    {
      if (window != 0)    /* The window is open - change all on-screen icons */
      {
        icon_flags flags = { 0 };
        flags.font.handle = fontarray->fonts[icons[i].flags.font.handle];
        Wimp_SetIconState(window, i, flags.value & FONT_MASK, FONT_MASK);
      }

      /* Change icon font handle in the window defn block */
      icons[i].flags.font.handle =
                          fontarray->fonts[icons[i].flags.font.handle];
    }
  }
}



extern void Window_ModeChange(void)
{
  windowrec       *record;
  template_record *tptr;
  font_defn       defn;
  font_array      fontconvert;
  int             i;

  if ((int)template_fontarray < 0)
    return;                       /* You aren't using outline fonts, stupid! */

  /*  Go through the template's font usage array...
   *  For each font we are using, re-find it at the same point size to get
   *  the font system to cache the font for the current screen mode.
   *  We fill in a conversion table which maps each old font handle to each
   *  new one (fontconvert.fonts[old_handle] = new_handle)
   */

  for (i = 0; i < 256; i++)
  {
    fontconvert.fonts[i] = 0;
    if (template_fontarray->fonts[i] > 0)       /* If we are using this font */
    {
      font_handle h = 0;

      Font_ReadDefn(i, &defn);             /* Find out its name and size ... */
                                           /* ...and get a new handle for it */
      Font_FindFont(&h, defn.name, defn.xsize, defn.ysize, 0, 0);
      fontconvert.fonts[i] = h;  /* (BTW, it's OK if the handle is the same) */
    }
  }

  /*  Replace all of the fonts we are using with the new handles
   *  Firstly, go through all of the Window_Create'd windows (open windows)
   *  and fix them... (Note that if any windows use fonts but were not
   *  Window_Create'd then
   *    a) they won't be fixed, and
   *    b) the font usage is probably now 0, so if the cache fills up,
   *       the font may be lost/replaced, and NASTY things will happen!
   */

  record = LinkList_FirstItem(&window_listanchor);
  while (record != NULL)
  {
    MapFontHandles((window_block *) record->memory,
                   record->window, &fontconvert);

    record = LinkList_NextItem(&record->header);
  }

  /*  Now do the template definitions themselves (otherwise the next time
   *  you open a window all the font handles will be wrong and won't you
   *  look silly?)
   */
  tptr = (template_record *) template_list.next;
  while (tptr != NULL)
  {
    MapFontHandles(tptr->windowdef, (window_handle) 0, &fontconvert);
    tptr = (template_record *) tptr->header.next;
  }

  /*  Lose all the old fonts and install new font list into Template module
   *  so that we know which fonts to lose next time, or when we quit.
   *  We Font_FindFont'd each font in the conversion table
   *  Thus, for each font handle in the conversion table, we increment its
   *  usage count. (Note that Font_LoseAllFonts() clears the table out to 0's)
   */

  Font_LoseAllFonts(template_fontarray);

  for (i = 0; i < 256; i++)
    if (fontconvert.fonts[i] != 0)
      template_fontarray->fonts[(unsigned int)fontconvert.fonts[i]]++;
}
