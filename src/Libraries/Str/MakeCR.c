/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Str.MakeCR.c
    Author:  Copyright © 1994 Tim Browse
    Version: 1.00 (06 Mar 1994)
    Purpose: Convert a 0-terminated (ASCIIZ) string to be CR-terminated.
*/

#include "DeskLib:Str.h"


void Str_MakeCR(char *s, int max_len)
{
  int i = 0;

  while ((i < max_len) && (s[i] != '\0'))
    i++;

  if (i < max_len)
    s[i] = '\15'; /* CR */
}

