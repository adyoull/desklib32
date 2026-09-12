/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    File.Size.c
    Author:  Copyright © 1993 Jason Howat
    Version: 1.00 (14 Aug 1993)
    Purpose: Sets the filetype of the given file
*/

#include "kernel.h"

#include "DeskLib:File.h"

extern void File_SetType(const char *filename, int type)
{
  _kernel_osfile_block osfile;

  osfile.load = type;
  _kernel_osfile(18, filename, &osfile);
}
