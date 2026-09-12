/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Template.Free.c
    Author:  Copyright © 1992 Jason Williams
             Thanks to John Winters for supplying the code that I hacked
             changed, hacked, rewrote, and then wrote again from scratch!
    Version: 1.10 (29 Mar 1992)
    Purpose: Loading, cacheing, and retrieval of window templates
*/

#include "TempDefs.h"



extern void Template_Free(window_block **windowdef)
/* Deallocates the memory used by a window definition */
{
  window_block *window;
  icon_block   *icons;
  unsigned int  icon;

  window = *windowdef;
  if (window->titleflags.data.indirected)
    free(window->title.indirecttext.buffer);

  icons = (icon_block *) ((int)window + sizeof(window_block));
  for (icon = 0; icon < window->numicons; icon++)
  {
    if (icons[icon].flags.data.indirected && icons[icon].flags.data.text)
    {
      free(icons[icon].data.indirecttext.buffer);
      if ((int) icons[icon].data.indirecttext.validstring > 0)
        free(icons[icon].data.indirecttext.validstring);
    }
  }

  free(window);
  *windowdef = NULL;
}
