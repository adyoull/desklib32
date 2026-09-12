/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    MemDefs.h
    Author:  Copyright © 1993, 1995 Jason Williams and Jason Howat
    Version: 2.00 (18 Jun 1995)
    Purpose: Dynamic memory management
 */


#ifndef __dl_memdef_h
#define __dl_memdef_h

#include "DeskLib:Mem.h"

typedef struct
{
  void **handle;                 /* The handle/anchor pointing to this block */
  int  prevrealsize;             /* Real block size of previous chunk        */
  int  realsize, datasize;       /* Chunk's real size and size of data-chunk */
} mem_header;


#ifdef __dl_mem_c
  extern mem_header *mem__lastchunk;
  extern int        mem__heap;
  extern int        mem__heapsize;
  extern int        mem__pagesize;
  extern int        mem__free;
#endif

#ifndef WORDALIGN
	/* Rounds an address up to the nearest word boundary */
	#define WORDALIGN(ADDR) (((ADDR) + 3) & (~3))
#endif

/* Rounds a datasize up to the smallest realsize that fits it */
#define CHUNKSIZE(DATA) ((int)((DATA) + 3 + sizeof(mem_header)) & (~3))

/* Returns TRUE if the given chunk is free */
#define ISFREE(CHUNK) ((CHUNK)->datasize == 0)

extern int Mem__HeapSize(int heapsize);
extern void Mem__ReduceSlot(void);
extern mem_header *Mem__NextChunk(mem_header *chunk);
extern mem_header *Mem__PrevChunk(mem_header *chunk);
extern BOOL Mem__FindChunk(mem_anchor *anchor, mem_header **chunk);
extern void Mem__SplitOffFreeChunk(mem_header *chunk);
extern void Mem__ShuffleDown(mem_header *start, mem_header *end);
extern void Mem__ShuffleUp(mem_header *start, mem_header *end);
extern void Mem__Amalgamate(mem_header **chunk);


/* #define MEM__DEBUG to include code to have debugging
 * information printed on stderr
 */
#undef MEM__DEBUG


#endif
