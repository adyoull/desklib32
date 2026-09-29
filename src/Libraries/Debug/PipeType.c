/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Debug1.c.uniquepipe
    Author:  Julian Smith, with clever taskwindow code from Paul Field and Cy Booker.
    Version: 0.00 (04 Jun 1995)
             0.10 (31 Aug 2007) Altered to work as part of main library
    Purpose: Provides a set of Debug_ function which send stuff to a unique
             file in a given directory.
*/


#include <stdarg.h>
#include <stdio.h>

/*
31/8/07 - not sure why this #undef was here?
#undef vsprintf
*/

#include "DeskLib:Error.h"
#include "DeskLib:WimpSWIs.h"

#include "DeskLib:Debug.h"

#include "DeskLib:Str.h"

#include "DebugDefs.h"

extern char debug__filename[256]; /* Set up in Debug.c */

BOOL Debug_InitialisePipetype(void)
/* Simply redirect stderr to a unique file in pipe:, and	*/
/* open a taskwindow which will *Type out stderr.		*/
/* Aren't taskwindows wonderful?				*/
{
  char command[320];
  snprintf(debug__filename, sizeof(debug__filename), "Pipe:$.DeskLib.%s", LeafName(tmpnam(NULL)));
  freopen(debug__filename, "w", stderr);
  setbuf(stderr, NULL);
  snprintf(command, sizeof(command),
           "taskwindow \"type %s\" -wimpslot 16k -name \"Debug output\" -quit",
           debug__filename);

  return Error_CheckSilent(Wimp_StartTask(command));
}

void Debug_PrintPipetype(const char *text)
{
  fputs(text, stderr);
}


