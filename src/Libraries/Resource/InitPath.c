/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Resource.InitPath.c
    Author:  Copyright © 1993 Jason Williams
    Version: 1.00 (15 Jul 1993)
             1.01 (01 Sep 2007) incorporate dl_resource_inited, use snprintf instead of strcat etc
    Purpose: Resource file (files within user application directory)
             management functions
*/


#include <stdio.h>
#include "DeskLib:Resource.h"

extern BOOL resource__autoinited; /* Set up in InitAuto.c */

extern void Resource_InitialisePath(const char *respath)
{
  snprintf(resource_pathname, sizeof(resource_pathname), "%s:", respath);

  resource__autoinited = TRUE;
}
