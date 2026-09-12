/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Mem.MoveAnchor.c
    Author:  Copyright © 1993 Jason Williams
    Version: 1.00 (15 Jul 1993)
    Purpose: Moving chunk anchors into different storage locations
*/

#include "MemDefs.h"


extern void Mem_MoveAnchor(mem_anchor *from, mem_anchor *to)
{
  mem_header *chunk;

  *to = NULL;
  if(Mem__FindChunk(from, &chunk))
  {
    *from = NULL;                                     /* Kill the old anchor */

    *to = (void *)((int)chunk + sizeof(mem_header));
    chunk->handle = to;                               /* Link in new anchor  */
  }
}
