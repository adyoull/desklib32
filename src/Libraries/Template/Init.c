/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Template.Init.c
    Author:  Copyright © 1992 Jason Williams
             Thanks to John Winters for supplying the code that I hacked
             changed, hacked, rewrote, and then wrote again from scratch!
    Version: 1.10 (29 Mar 1992)
             1.11 (21 Aug 2007) Removed need to explicitly call Template_Initialise
    Purpose: Loading, caching, and retrieval of window templates
*/


#include "TempDefs.h"
#include "DeskLib:Core.h" /* For BOOL */

BOOL template_initialised = FALSE;

extern void Template_Initialise(void)
{
  if (!template_initialised) /* In case user calls this fn manually, twice... */
    LinkList_Init(&template_list);
  template_initialised = TRUE;
}
