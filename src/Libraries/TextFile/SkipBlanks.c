/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    TextFile.SkipBlanks.c
    Author:  Copyright © 1992 Jason Williams
    Version: 1.00 (07 Apr 1992)
    Purpose: Generic textfile-handling routines
*/


#include <stdio.h>

#include "DeskLib:Core.h"
#include "DeskLib:TextFile.h"

/* Avoid problems with macros accessing FILE * internals, that would
   make Desklib tied to either SharedCLibrary or Unixlib */
#undef getc
#undef feof

extern void TextFile_SkipBlanks(FILE *infile)
{
  char ch = 9;

  while (!feof(infile) && (ch == 32 || ch == 9 || ch == '\n'))
    ch = getc(infile);
  ungetc(ch, infile);
}
