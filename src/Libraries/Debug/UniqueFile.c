/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Debug.c.uniquefile
    Author:  Julian Smith
    Version: 0.00 (04 Jun 1995)
             0.10 (31 Aug 2007) Altered to work as part of main library
    Purpose: Provides a set of Debug_ function which send stuff to a unique
             file found using tmpnam().
*/


#include <stdarg.h>
#include <stdio.h>

/*
31/8/07 - not sure why this #undef was here?
#undef vsprintf
*/

#include "DeskLib:Error.h"
#include "DeskLib:Str.h"
#include "DeskLib:Debug.h"

#include "DebugDefs.h"


static FILE *debug__file;

extern char debug__filename[256]; /* Set up in Debug.c */

BOOL Debug_InitialiseUniqueFile(void)
{
  tmpnam(debug__filename);
  debug__file = fopen(debug__filename, "w");
  if (!debug__file)
  {
    Error_Report(1, "Debug_Initialise can't open output file '%s'\n", debug__filename);
    return TRUE;
  }

  if (setvbuf(debug__file, NULL, _IONBF, 0)) /* Turn off buffering	*/
    Error_Report(1, error_PLACE "Couldn't turn buffering off for output debug file");

  return FALSE;
}


void Debug_PrintUniqueFile(const char *text)
{
  fputs(text, debug__file);
}


