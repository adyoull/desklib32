/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    File.IsDir.c
    Author:  Copyright © 1994 Tim Browse
    Version: 1.01 (03 Jun 1995)
    Purpose: Checks to see if the given file is a directory.
    Mods:    Returns TRUE for images as well as directories.
*/

#include "DeskLib:Core.h"
#include "DeskLib:File.h"
#include "DeskLib:SWI.h"

extern BOOL File_IsDirectory(const char *pathname)
{
  unsigned type;

  /* Get the file information */

  SWI(2, 1, SWI_OS_File, 17, pathname, &type);

  return ( (type & 2) ? TRUE : FALSE); /* 2 => directory found, 3 => image */
}

