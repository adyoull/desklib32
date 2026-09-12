/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Filing.SingleFull.c
    Author:  Copyright © 1994, 1995 Sergio Monesi
    Version: 1.01 (6 Jun 1995)
    Purpose: Reads a single directory entry (full info version)
    Mods:    6 June 1995 - changed procedure names to a more
                           DeskLib-compliant style
*/

#include "DeskLib:Core.h"
#include "DeskLib:Filing.h"

os_error *Filing_SingleFullDirEntry(const char *filename,
                                    filing_fulldirentry *buf, int size)
{
 char pathname[256];
 const char *leafname;

 Filing_GetPathname(filename,pathname);
 leafname = Filing_FindLeafname(filename);

 return Filing_SingleFullDirEntry2(pathname,buf,size,leafname);
}
