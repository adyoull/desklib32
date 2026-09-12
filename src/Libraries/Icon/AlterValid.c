/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Icon.AlterValid.c
    Author:  Copyright 1994 Tony Howat
    Version: 1.00 (2 Aug 1994)
    Purpose: Changes an icon's validation string - makes pressy slabbed
             borders easy.
*/

#include <string.h>

#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"


extern void Icon_AlterValidation(window_handle window, icon_handle icon,
                                 const char *newvalidation)
{
  icon_block wimpicon;

  Wimp_GetIconState(window, icon, &wimpicon);

  if (!strcmp(wimpicon.data.indirecttext.validstring, newvalidation))
     return;

  strcpy(wimpicon.data.indirecttext.validstring, newvalidation);

  Wimp_SetIconState(window, icon, 0, 0);
}

