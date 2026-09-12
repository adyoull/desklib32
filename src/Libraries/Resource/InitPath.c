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
    Purpose: Resource file (files within user application directory)
             management functions
*/


#include <string.h>
#include "DeskLib:Resource.h"


extern void Resource_InitialisePath(const char *respath)
{
  strncpy(resource_pathname, respath, 30);
  resource_pathname[30] = '\0';               /* Ensure string is terminated */
  strcat(resource_pathname, ":");
}
