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
             1.01 (5 Sep 2007) Changed strcpy to strncpy
             1.02 (9 Dec 2007) Updated to return int with useful info
    Purpose: Changes an icon's validation string - makes pressy slabbed
             borders easy.
*/

#include <string.h>

#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"
#include "DeskLib:Str.h"


int Icon_AlterValidation(window_handle window, icon_handle icon,
                                 const char *newvalidation)
{
  icon_block wimpicon;
  int diff, len_existing;

  Wimp_GetIconState(window, icon, &wimpicon);

  /* Only proceed if icon could possibly have a validation string */
  if (!wimpicon.flags.data.indirected)
    return 0;
  else
  {
    /* Get length of existing validation string, for info */
    len_existing = strlencr(wimpicon.data.indirecttext.validstring);

    /* If user called us only to find length of existing string, return here */
    if (newvalidation == NULL)
      return len_existing;

    /* Check old vs. new strings are different - if not, do nothing */
    if (!strcmpcr(wimpicon.data.indirecttext.validstring, newvalidation))
       return 0;

    /* Return difference between strings, for info */
    diff = len_existing - strlencr(newvalidation);

    /* Note: nothing to prevent buffer overrun */
    strcpy(wimpicon.data.indirecttext.validstring, newvalidation);

    Wimp_SetIconState(window, icon, 0, 0);
  }

  return diff;
}

