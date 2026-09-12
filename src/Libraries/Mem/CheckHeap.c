/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Mem.CheckHeap.c
    Author:  Copyright © 1994, 1995 Jason Howat
    Version: 2.00 (18 Jun 1995)
    Purpose: Dynamic memory manager - checks validity of heap
*/

#define __dl_mem_c
#include "MemDefs.h"

#include "DeskLib:Error.h"

#ifdef MEM__DEBUG
#include <stdio.h>
#endif


extern BOOL Mem_CheckHeap(void)
/*  Returns TRUE if the heap data structure is valid (i.e. the links are all
 *  intact and anchors are consistent)
 */
{
  mem_header *chunk = (mem_header *) mem__heap,
             *next = chunk,
             *end_of_heap = (mem_header *) (mem__heap + mem__heapsize);
  int         free = 0;

  while(TRUE)
  {
#ifdef MEM__DEBUG
fprintf(stderr, "[%p,%p,%8x,%8x,%8x]\n",
                chunk,
                chunk->handle,
                chunk->prevrealsize,
                chunk->realsize,
                chunk->datasize);
fflush(stderr);
#endif

    if(ISFREE(chunk))
      free += chunk->realsize;
    else
      free += chunk->realsize - CHUNKSIZE(chunk->datasize);

    if(!ISFREE(chunk) &&
       ((int) *(chunk->handle) != ((int) chunk + (int)sizeof(mem_header))))
    {
      Error_Report(0, "Mem heap corrupt: Inconsistent chunk handle and anchor (chunk %p)",
                      chunk);
#ifdef MEM__DEBUG
fprintf(stderr, "Mem heap corrupt: Inconsistent chunk handle and anchor (chunk %p)\n", chunk);
fflush(stderr);
#endif
      return(FALSE);
    }

    if(chunk->realsize < chunk->datasize + (int)sizeof(mem_header))
    {
      Error_Report(0, "Mem heap corrupt: Data larger than chunk (chunk %p)",
                      chunk);
#ifdef MEM__DEBUG
fprintf(stderr, "Mem heap corrupt: Data larger than chunk (chunk %p)\n", chunk);
fflush(stderr);
#endif
      return(FALSE);
    }

    next = (mem_header *) ((int)chunk + chunk->realsize);
    if(next >= end_of_heap)
      break;

    if(chunk >= next)
    {
      Error_Report(0, "Mem heap corrupt: Backward link (chunk %p)",
                      chunk);
#ifdef MEM__DEBUG
fprintf(stderr, "Mem heap corrupt: Backward link (chunk %p)\n", chunk);
fflush(stderr);
#endif
      return(FALSE);
    }

    if(chunk->realsize != next->prevrealsize)
    {
      Error_Report(0, "Mem heap corrupt: Chunk link(s) corrupt (chunks %p and %p)",
                      chunk, next);
#ifdef MEM__DEBUG
fprintf(stderr, "Mem heap corrupt: Chunk link(s) corrupt (chunks %p and %p)\n", chunk, next);
fflush(stderr);
#endif
      return(FALSE);
    }

    if(ISFREE(chunk) && ISFREE(next))
    {
      Error_Report(0, "Mem heap corrupt: Two consecutive free chunks (chunks %p and %p)",
                      chunk, next);
#ifdef MEM__DEBUG
fprintf(stderr, "Mem heap corrupt: Two consecutive free chunks (chunks %p and %p)\n", chunk, next);
fflush(stderr);
#endif
      return(FALSE);
    }

    chunk = next;
  }

  if(next > end_of_heap)
  {
    Error_Report(0, "Mem heap corrupt: Last chunk too big");
#ifdef MEM__DEBUG
fprintf(stderr, "Mem heap corrupt: Last chunk too big\n");
fflush(stderr);
#endif
    return(FALSE);
  }

  if(chunk != mem__lastchunk)
  {
    Error_Report(0, "Mem heap corrupt: mem__lastchunk not set correctly (is %p, should be %p)",
                    mem__lastchunk, chunk);
#ifdef MEM__DEBUG
fprintf(stderr, "Mem heap corrupt: mem__lastchunk not set correctly (is %p, should be %p)\n", mem__lastchunk, chunk);
fflush(stderr);
#endif
    return(FALSE);
  }

#ifdef MEM__DEBUG
fprintf(stderr, "mem__free: %8x\n---\n",mem__free);
fflush(stderr);
#endif

  if(free != mem__free)
  {
    Error_Report(0, "Mem heap corrupt: mem__free not consistent with actual free space (is %x, should be %x)",
                    mem__free, free);
#ifdef MEM__DEBUG
fprintf(stderr, "Mem heap corrupt: mem__free not consistent with actual free space (is %x, should be %x)\n", mem__free, free);
fflush(stderr);
#endif
    return(FALSE);
  }

  return(TRUE);
}
