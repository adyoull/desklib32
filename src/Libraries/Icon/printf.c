/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Icon.printf.c
    Author:  Copyright © 1992, 1993, 1994 Jason Williams
             Concept by Edouard Poor
    Version: 1.02 (12 Mar 1994)
             1.03 (5 Sep 2007) Changed vsprintf to vsnprintf
             1.04 (22 Oct 2007) Changes name to DeskLib style
    Purpose: Do a "printf" into an icon's indirected text string
*/

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/* commented out 5/9/07 - why was it here?
#undef vsprintf
*/

#include "DeskLib:Wimp.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"

extern void Icon_Printf(window_handle window, icon_handle icon,
                        const char *format, ...)
{
  va_list    argp;
  char       temp[512];   /* Longer strings will break this call */
  icon_block istate;
  caret_block caret;
  int         len;

  Wimp_GetIconState(window, icon, &istate);
  if (!istate.flags.data.indirected)
    return;

  va_start(argp,format);
  vsnprintf(temp, sizeof(temp), format, argp);
  strncpy(istate.data.indirecttext.buffer, temp,
          istate.data.indirecttext.bufflen - 1);
  istate.data.indirecttext.buffer[istate.data.indirecttext.bufflen - 1] = '\0';
  va_end(argp);
  temp[sizeof(temp)-1] = '\0';

  /* Ensure caret isn't beyond end of text */
  Wimp_GetCaretPosition( &caret );
  if ( caret.window == window && caret.icon == icon )
  {
    len = strlen( istate.data.indirecttext.buffer );
    if ( caret.index > len )
    {
      caret.index = len;
      Wimp_SetCaretPosition( &caret );
    }
  }

  Wimp_SetIconState(window, icon, 0, 0);
}
