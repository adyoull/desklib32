/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Str.MakeASCIIZ.c
    Author:  Copyright © 1994 Tim Browse
    Version: 1.00 (06 Mar 1994)
    Purpose: Convert a CR-terminated string to be 0-terminated (ASCIIZ).
*/

#include "DeskLib:Str.h"


void Str_MakeASCIIZ(char *s, int max_len)
{
  int i = 0;

  while ((i < max_len) && (s[i] > 31))
    i++;

  if (i < max_len)
    s[i] = '\0'; /* Null terminate it */
}
