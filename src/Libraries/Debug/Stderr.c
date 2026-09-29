/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Debug1.c.Debug1
    Author:  Julian Smith
    Version: 0.00 (04 Jun 1995)
             0.10 (31 Aug 2007) Altered to work as part of main library
    Purpose: Provides a set of Debug_ function which send stuff to stderr
*/


#include <stdarg.h>
#include <stdio.h>

/*
31/8/07 - not sure why this #undef was here?
#undef vsprintf
*/

#include "DeskLib:Debug.h"

#include "DebugDefs.h"

void Debug_PrintStderr(const char *text)
{
  fputs(text, stderr);
}


