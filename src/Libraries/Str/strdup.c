/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Str.strdup.c
    Author:  Copyright © 1994 Tim Browse
    Version: 1.00 (06 Mar 1994)
    Purpose: Make a copy of a 0-terminated string.
*/

#include <string.h>
#include <stdlib.h>

#include "DeskLib:Str.h"

char *strdup(const char *src)
{
  int   len;
  char *dest;

  if (src == NULL)
    return NULL;

  len = strlen(src) + 1;

  dest = malloc(len);

  if (dest == NULL)
    return NULL;

  memcpy(dest, src, len-1);

  /* make sure string is zero terminated */
  dest[len-1] = '\0';

  return dest;
}
