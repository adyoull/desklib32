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
    Purpose: Resource file (files within user application directory)
             management functions
*/


#include <string.h>
#include "DeskLib:Resource.h"
#include "DeskLib:Str.h"

extern void Resource_Initialise(const char *respath)
{
  resource_pathname[0] = '<';
  strncat(resource_pathname, respath, 23);
  resource_pathname[24] = '\0';               /* Ensure string is terminated */
  strcat(resource_pathname, "$Dir>.");

}
