/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Resource.c
    Author:  Copyright © 1992 Jason Williams
    Version: 1.01 (06 May 1993)
             1.02 (01 Sep 2007) incorporate dl_resource_inited, use snprintf instead of strcat etc.
    Purpose: Resource file (files within user application directory) management functions
*/


#include <stdio.h>
#include "DeskLib:Resource.h"

extern BOOL resource__autoinited; /* Set up in InitAuto.c */

extern void Resource_Initialise(const char *respath)
{
  snprintf(resource_pathname, sizeof(resource_pathname), "<%s$Dir>.",respath);
  resource__autoinited = TRUE;
}
