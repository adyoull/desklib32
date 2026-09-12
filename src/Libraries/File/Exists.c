/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    File.Exists.c
    Author:  Copyright © 1993,1994 Jason Williams, Jason Howat
    Version: 1.02 (25 Sep 2002) (AJPS)
    Purpose: Checks if the file exists at all, and if it can be termed a
             "file" - directories are excluded, image files are not.
*/


#include "DeskLib:File.h"
#include "kernel.h"


extern BOOL File_Exists(const char *filename)
{
  _kernel_osfile_block osfile;

  return ((_kernel_osfile(17, filename, &osfile) & 1) == 1);
}
