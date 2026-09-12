/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Filing.Single.c
    Author:  Copyright © 1995 Sergio Monesi
    Version: 1.00 (28 Jul 1995)
    Purpose: Reads a single directory entry
*/

#include "DeskLib:Core.h"
#include "DeskLib:Filing.h"

os_error *Filing_SingleDirEntry(const char *filename, filing_direntry *buf,
                                int size)
{
 char pathname[256];
 char *leafname;

 Filing_GetPathname(filename,pathname);
 leafname = Filing_FindLeafname(filename);

 return Filing_SingleDirEntry2(pathname,buf,size,leafname);
}
