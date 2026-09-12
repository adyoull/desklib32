/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Mem.Size.c
    Author:  Copyright © 1993 Jason Williams
    Version: 1.00 (12 May 1993)
    Purpose: Dynamic memory manager - return block's size
*/

#include "MemDefs.h"


extern int Mem_Size(mem_anchor *anchor)
/*  Returns the size of the DATA held in the chunk (i.e the amount of memory
 *  allocated by the user, not the actual amount of memory Mem is using for it)
 */
{
  mem_header *chunk;

  if(Mem__FindChunk(anchor, &chunk))
    return(chunk->datasize);

  return(0);
}
