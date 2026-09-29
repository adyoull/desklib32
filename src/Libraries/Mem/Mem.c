/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Mem.c
    Author:  Copyright © 1993, 1994, 1995 Jason Williams and Jason Howat
    Version: 2.00 (18 Jun 1995)
    Purpose: Dynamic memory management

 *  Details for use of Mem are in Mem.h
 *  Ideas that may be added to Mem.c in future are in Docs.ModuleNote.Mem
 *
 *  NOTE that some functions and variables are Mem__xxxx: These are NOT
 *  intended for external use outside Mem_ Functions, and are only externs
 *  so that Mem does not need to be linked with as one huge chunk.
 *
 *  Details of how this code works:
 *  Mem memory chunks are held in a 'heap' of memory above the area grabbed
 *  by the SharedCLibrary for mallocs and stackspace, extending up to the
 *  end of the WimpSlot, which is adjusted up & down as necessary.
 *
 *  The Mem heap is simple, and is *exactly* filled by a contiguous set of
 *  chunks. This is laid out as follows:
 *
 *    [H] [H|--data--:P] ... [H|-----data-----:P] [H]
 *
 *  Where H is a chunk header containing
 *    + the size of the previous chunk, including H and P
 *       (for moving backwards through the chain of chunks)
 *
 *    + the size of this chunk, including H and P
 *       (for moving forwards through the chain of chunks)
 *
 *    + the size of the user data held in this chunk
 *      Note that userdatasize + H + P does not necessarily  == chunksize
 *
 *    + the anchor of this chunk
 *       (a pointer to the user's pointer to the chunk)
 *       This is used for 2 things: To update the user's anchor when the chunk
 *       is moved, and also to check that all accesses are legal.
 *
 *  P is padding to word-align the block. Note that the padding may be
 *  anywhere between 0 and 20 bytes in size - normally it will be 0..3 to
 *  get word alignment, but if a block is allocated into a free space which
 *  is not big enough for that block plus a new free block, we'll swallow
 *  the whole freespace into the newly allocated block to ensure the continued
 *  viability of the heap. Note also that during some operations, chunks
 *  may temporarily exist with P fields of any size at all.
 */

#include <stdlib.h>
#include <string.h>

#include "kernel.h"

#define __dl_mem_c
#include "MemDefs.h"

#ifdef MEM__DEBUG
#include <stdio.h>
#endif

#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Error.h"


/* --- External interface - global variables ------------------------------- */
int mem_autocompact = mem_FASTCOMPACT;     /* Enable/disable auto-compaction */


/* --- Internal data ------------------------------------------------------- */
mem_header *mem__lastchunk = NULL;
int mem__heap         = -1;      /* Base address of heap (malloc high water) */
int mem__heapsize     = -1;      /* Size of the heap                         */
int mem__pagesize     = -1;      /* Size of pages used by wimp               */
int mem__free         = -1;      /* Number of free bytes in heap             */


/* --- Internal Function definitions --------------------------------------- */

extern int Mem__HeapSize(int heapsize)
/*  Reads/Writes the heap size value
 *  pass in heapsize == -1 to just read the current value
 */
{
  int nextslot = -1;
  int freepool = -1;
  int slot     = heapsize;

  if (slot >= 0) slot += mem__heap - 0x8000;  /* Work out new wimp slot size */
  Wimp_SlotSize(&slot, &nextslot, &freepool);

  return(slot - mem__heap + 0x8000);          /* Return new heap size        */
}



extern void Mem__ReduceSlot(void)
/*  Attempts to shrink the heap by returning all free memory in the lastchunk
 *  back to the Wimp.  Ensure we don't nuke any data in the last chunk!
 */
{
  int prev_size;

  if (mem__lastchunk->realsize > CHUNKSIZE(mem__lastchunk->datasize) + 16)
  {
    prev_size = mem__heapsize;
    mem__heapsize = Mem__HeapSize((int) mem__lastchunk - mem__heap +
                                  CHUNKSIZE(mem__lastchunk->datasize) + 16);
    mem__lastchunk->realsize = mem__heap + mem__heapsize - (int)mem__lastchunk;
    mem__free -= prev_size - mem__heapsize;
  }
}



extern mem_header *Mem__NextChunk(mem_header *chunk)
/*  Returns a pointer to the next chunk in the heap. Note that if
 *  'chunk' is the last chunk in the heap, it will return 'chunk'.
 */
{
  if (chunk == mem__lastchunk)
    return(chunk);

  return((mem_header *) ((int) chunk + chunk->realsize));
}


extern mem_header *Mem__PrevChunk(mem_header *chunk)
/*  Returns a pointer to the previous chunk in the heap. Note that if
 *  'chunk' is the first chunk in the heap, it will return 'chunk'.
 */
{
  return((mem_header *) ((int) chunk - chunk->prevrealsize));
}


extern BOOL Mem__FindChunk(mem_anchor *anchor, mem_header **chunk)
/*  Checks that the header is OK by comparing the handle to the
 *  (supposedly identical) handle stored in the chunk header.
 *  Returns the pointer to the chunk header, and TRUE if it is OK.
 */
{
  if (*anchor == NULL) return(FALSE);

  *chunk = (mem_header *) (((int) *anchor) - sizeof(mem_header));
  return((*chunk)->handle == anchor);
}



void Mem__Amalgamate(mem_header **chunk)
/*  Tries to merge this free chunk with the previous chunk, if it's free.
 */
{
  mem_header *merge, *prev, *next;

  merge = *chunk;
  if (!ISFREE(merge) || (int) merge <= mem__heap) return;

  prev = Mem__PrevChunk(merge);
  if (prev != merge && ISFREE(prev))                /* Amalgamate free chunks */
  {
    prev->realsize += merge->realsize;              /* Extend prev chunk      */
    *chunk = prev;                                 /* Return new free chunk  */

    if (merge == mem__lastchunk)                    /* Update lastchunk ptr   */
      mem__lastchunk = prev;
    else
    {                                              /* Fix backward 'link'    */
      next = Mem__NextChunk(prev);
      next->prevrealsize = prev->realsize;
    }
  }
}


extern void Mem__SplitOffFreeChunk(mem_header *chunk)
/*  Checks if it is possible to split off enough memory from the end of this
 *  chunk to create a new free chunk - if there is, it does the split.
 */
{
  if(ISFREE(chunk))
    return;

  if((chunk->realsize - chunk->datasize) >= (int)(sizeof(mem_header) + 16))
  {
    mem_header *freech;
    int         oldchunksize = chunk->realsize,
                newchunksize;

    /*  There is enough wastage at the end of the chunk to create a new free
     *  chunk of 16 or more bytes dataspace at the end of it
     */

    freech = (mem_header *)WORDALIGN((int)chunk + chunk->datasize + sizeof(mem_header));

    newchunksize         = (int)freech - (int)chunk;
    chunk->realsize      = newchunksize;
    freech->realsize     = oldchunksize - newchunksize;
    freech->prevrealsize = newchunksize;
    freech->datasize     = 0;
    freech->handle       = NULL;

    if (mem__lastchunk == chunk)
      mem__lastchunk = freech;
    else
    {
      chunk = Mem__NextChunk(freech);
      chunk->prevrealsize = freech->realsize;

      Mem__Amalgamate(&chunk);         /* Ensure not 2 free chunks in a row */
    }
  }

  if (mem_autocompact == mem_FULLCOMPACT)
    Mem_Compact();
}


extern void Mem__ShuffleUp(mem_header *start, mem_header *end)
/*  Moves all of the used blocks in the range (start < block < end) to a
 *  consecutive run at the end of the given area, fixing anchors as it goes,
 *  and appending the collected freespace to the first block.
 */
{
  mem_header *source,
             *dest = end,
             *next,
             *m_lc = mem__lastchunk;

#ifdef MEM__DEBUG
fprintf(stderr, "shuffle up: start %p, end %p\n", start, end);
fflush(stderr);
#endif

  if(end <= m_lc)
    source = (mem_header *)((int)end - end->prevrealsize);
  else
    source = m_lc;

  while(source > start)
  {
    next = (mem_header *)((int)source - source->prevrealsize);

    if(!ISFREE(source))
    {
      int blocksize = CHUNKSIZE(source->datasize);
      if(dest <= m_lc)
        dest->prevrealsize = blocksize;

      dest = (mem_header *)((int)dest - blocksize);
      memmove(dest, source, blocksize);         /* move chunk */

      dest->realsize = blocksize;
      *(dest->handle) = (void *)((int)dest + sizeof(mem_header));

      if(end > m_lc)                            /* new lastchunk */
        mem__lastchunk = m_lc = end = dest;
    }

    source = next;
  }

  start->realsize = (int)dest - (int)start;
  if(dest <= m_lc)
    dest->prevrealsize = start->realsize;
  else  /* no used blocks in run */
    mem__lastchunk = start;
}


extern void Mem__ShuffleDown(mem_header *start, mem_header *end)
/*  Moves all of the used blocks in the range (start <= block < end) to a
 *  consecutive run at the beginning of the given area, fixing anchors as it
 *  goes.
 */
{
  int         prevblocksize = start->prevrealsize;
  mem_header *source = start,
             *dest = start,
             *next;

#ifdef MEM__DEBUG
fprintf(stderr, "shuffle down: start %p, end %p\n", start, end);
fflush(stderr);
#endif

  while(source < end)
  {
    next = (mem_header *)((int)source + source->realsize);

    if(!ISFREE(source))
    {
      int currblocksize = CHUNKSIZE(source->datasize);

      /* shift block down */
      memmove(dest, source, currblocksize);

      dest->prevrealsize = prevblocksize;
      dest->realsize = currblocksize;
      prevblocksize = currblocksize;
      *(dest->handle) = (void *)((int)dest + sizeof(mem_header));
      dest = (mem_header *)((int)dest + dest->realsize);
    }

    source = next;
  }

  /* fix up free block at end */
  dest->handle = NULL;
  dest->prevrealsize = prevblocksize;
  dest->realsize = (int)end - (int)dest;
  dest->datasize = 0;

  /* fix up block after shuffled run of blocks */
  if(end <= mem__lastchunk)
    end->prevrealsize = dest->realsize;
  else
    mem__lastchunk = dest;  /* free block is last one in heap */
}



/* --- External Function definitions --------------------------------------- */

static BOOL Mem__TryAlloc(mem_anchor *anchor, int numbytes)
/*  Allocates 'numbytes' of memory, placing a pointer to this data in
 *  the given anchor. The memory returned always starts at a word-aligned
 *  address, though need not be an integral number of words long.
 *
 *  Note that this implementation provides a 'best fit' strategy, that is
 *  it searches for the free chunk of the *closest* size to that needed
 *  which can be slow if there are a lot of free chunks. However, this
 *  is still pretty quick, especially if you call Mem_Compact reasonably
 *  often to ensure the heap is kept compact.
 *
 *  Alternative strategies that you might prefer to implement for yourself
 *  are:
 *    + First fit: Find the first large enough free space
 *                 Faster than best fit, but generally less effective.
 *
 *    + Grab:      Just alloc at the end of the heap, and rely on (compacts
 *                 helping to keep) enough free memory in the Wimp Pool
 *                 to always be able to satisfy memory requests.
 *                 (This is very fast, and minimises the code in this function)
 *                 [RECOMPILE with FASTALLOC defined for this scheme]
 */
{
  mem_header *chunk, *bestfit = NULL,
             *start, *end,
             *best_start = NULL, *best_end = NULL,
             *end_of_heap = (mem_header *)(mem__heap + mem__heapsize);
  int         bestfit_size = 0,
              best_used = 0,
              best_free = 0,
              truesize = numbytes;

  *anchor  = NULL;
  if (numbytes <= 0) return(FALSE);

  numbytes = CHUNKSIZE(numbytes);

#ifndef FASTALLOC
  if(mem__free >= numbytes)  /* only search if free space exists */
  {
    if(mem_autocompact == mem_NOCOMPACT)
    {
      /*
       *  Find the best fit free-chunk in the heap. (i.e. search for the smallest
       *  chunk which is >= numbytes)
       */
      chunk = (mem_header *) mem__heap;

      while (chunk <= mem__lastchunk)
      {
        if (ISFREE(chunk) && chunk->realsize >= numbytes)
        {
          if (chunk->realsize == numbytes)        /* Don't bother continuing */
          {
            bestfit = chunk;
            break;
          }

          if (bestfit == NULL || bestfit_size > chunk->realsize)
          {
            bestfit = chunk;
            bestfit_size = bestfit->realsize;
          }
        }
        chunk = Mem__NextChunk(chunk);
      }
    }
    else
    {
      int free_count = 0,
          used_count = 0;

      /*
       *  Scan for the run of blocks that require the least amount of shifting
       *  to create a free block of the needed size.
       */
      /* alternately advance start and end, keeping free_count >= numbytes
         and record start,end in best_start,best_end for pair with minimum
         used_count */

      start = (mem_header *) mem__heap;
      end = (mem_header *) mem__heap;

      best_used = mem__heapsize;

      while(1)
      {
        int used;

        while((free_count < numbytes) && (end < end_of_heap))
        {
          if(ISFREE(end) && end->realsize >= numbytes)
          {
            if(end->realsize == numbytes)       /* Don't bother continuing */
            {
              bestfit = end;
              goto mem__best_block_found;  /* yeah... I know! */
            }

            if(bestfit == NULL || bestfit_size > end->realsize)
            {
              bestfit = end;
              bestfit_size = bestfit->realsize;
            }
          }

          if(!ISFREE(end))
          {
            used = CHUNKSIZE(end->datasize);
            used_count += used;
          }
          else
            used = 0;
          free_count += end->realsize - used;

          end = (mem_header *)((int)end + end->realsize);
        }

        if(free_count < numbytes)
          break;      /* reached end of heap */
        else if(used_count < best_used)
        {     /* found a better run of blocks (less work to compact) */
          best_start = start;
          best_end = end;
          best_used = used_count;
          best_free = free_count;
        }

        if(!ISFREE(start))
        {
          used = CHUNKSIZE(start->datasize);
          used_count -= used;
        }
        else
          used = 0;
        free_count -= start->realsize - used;

        start = (mem_header *)((int)start + start->realsize);
      }

mem__best_block_found:;     /* used to stop searching when perfect sized block is found */

    }
  }

  if (bestfit == NULL)      /* No large enough chunks - must extend the heap */
#endif

  {
    int needed = numbytes,
        prevsize;

    if(ISFREE(mem__lastchunk))
      needed -= mem__lastchunk->realsize;

    /* last chunk is bigger than needed, should have been found earlier */
    if(needed < 0)
      Error_ReportFatalInternal(0, "Unexpected failure in Mem_Alloc");

    prevsize = mem__heapsize;
    mem__heapsize = Mem__HeapSize(mem__heapsize + needed + 16);
    mem__free += mem__heapsize - prevsize;
    mem__lastchunk->realsize = mem__heap + mem__heapsize - (int)mem__lastchunk;

    Mem__SplitOffFreeChunk(mem__lastchunk);

    if(mem__lastchunk->realsize >= numbytes && ISFREE(mem__lastchunk))
      bestfit = mem__lastchunk;
    else if(mem_autocompact != mem_NOCOMPACT)
    {
      /* couldn't extend heap enough -- check if compaction will help */

      if(mem__free >= numbytes)
      {
        int free_count,
            used_count,
            used;
        mem_header *hstart = mem__lastchunk;

        if(ISFREE(hstart))
        {
          used_count = 0;
          free_count = hstart->realsize;
        }
        else
        {
          used_count = CHUNKSIZE(hstart->datasize);
          free_count = hstart->realsize - used_count;
        }

        while((free_count < numbytes) && ((int)hstart > mem__heap))
        {
          hstart = (mem_header *)((int)hstart - hstart->prevrealsize);

          if(!ISFREE(hstart))
          {
            used = CHUNKSIZE(hstart->datasize);
            used_count += used;
          }
          else
            used = 0;
          free_count += hstart->realsize - used;
        }

        if((free_count >= numbytes) && (used_count < best_used))
        {     /* found a better run of blocks (less work to compact) */
          best_start = hstart;
          best_end = (mem_header *)(mem__heap + mem__heapsize);
          best_used = used_count;
          best_free = free_count;
        }
      }
    }
  }

  /* last chance -- compact if allowed and it would create enough space */
  if((bestfit == NULL) &&
     (mem_autocompact != mem_NOCOMPACT) &&
     (best_start != NULL))
  {
    Mem__ShuffleDown(best_start, best_end);
    bestfit = (mem_header *)((int)best_end - best_free);
#ifdef MEM__DEBUG
fprintf(stderr, "last chance: start %p, end %p, chunk %p\n",
                best_start, best_end, bestfit);
fflush(stderr);
#endif
  }

  if (bestfit == NULL)                      /* Unable to allocate the memory */
  {
    Mem__ReduceSlot();
    return(FALSE);
  }

  *anchor = (void *) ((int) bestfit + sizeof(mem_header));
  bestfit->handle = anchor;
  bestfit->datasize = truesize;
  mem__free -= numbytes;

  Mem__SplitOffFreeChunk(bestfit);         /* Return any free space for use */

  return(TRUE);
}

extern BOOL Mem_Alloc(mem_anchor *anchor, int numbytes)
{
  return Mem__TryAlloc(anchor, numbytes);

  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   *                                                                       *
   *       Add code to call out-of-space handlers when unsuccessful        *
   *                                                                       *
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}


extern void Mem_Free(mem_anchor *anchor)
{
  mem_header *chunk;

  if (Mem__FindChunk(anchor, &chunk))
  {
    mem__free += CHUNKSIZE(chunk->datasize);

    chunk->datasize = 0;                           /* Mark this chunk free   */
    *anchor         = NULL;                        /* Kill the user's anchor */

    Mem__Amalgamate(&chunk);
    if (chunk != mem__lastchunk)
    {
      chunk = Mem__NextChunk(chunk);
      Mem__Amalgamate(&chunk);
    }

    if (mem_autocompact == mem_FULLCOMPACT)
      Mem_Compact();      /* Compact also does a ReduceSlot */
    else
      Mem__ReduceSlot();
  }
}

#ifndef __GNUC__
#pragma no_check_stack
#endif

static int Mem_DontBudge(int n, void **a)
/*  Function to register with _kernel_register_slotextend to refuse to allow
 *  SharedCLib to allocate more memory. (identical to Flex's flex_dontbudge)
 *  Don't call this function - pass to _k*_r*_slotextend instead. Note that
 *  Mem_Initialise does this for you automatically anyway, so unless you have
 *  a Mem_Budge function, you shouldn't have to worry about this at all.
 */
{
  n = n;
  a = a;
  return(0);                                    /* number of bytes allocated */
}

#ifndef __GNUC__
#pragma check_stack
#endif

extern BOOL Mem_Initialise(void)
/*  Initialises the Mem memory manager
 *  Returns FALSE if it fails - if this occurs, Mem will not operate and
 *  you should not try calling any Mem FNs. Basically you're stuffed.
 *  (Generally you should report the "Insufficient memory or not within
 *   *desktop world" error message and exit)
 */
{
#ifdef __UNIXLIB_FEATURES_H

  return(FALSE);

#else
  _kernel_register_slotextend(Mem_DontBudge);

  if (mem__heap == -1)    /* This will be > 0 if we have already initialised */
  {
    int nextslot = -1,
        freepool = -1;

    Wimp_SlotSize(&mem__heap, &nextslot, &freepool);
    mem__heap = WORDALIGN(mem__heap);
    mem__heap += 0x8000;    /* Add application base address to get heap base */

    mem__heapsize  = Mem__HeapSize(sizeof(mem_header) + 16);
    if (mem__heapsize < (int)sizeof(mem_header))
      return(FALSE);

    mem__lastchunk = (mem_header *) mem__heap;
    mem__lastchunk->realsize     = mem__heapsize;
    mem__lastchunk->datasize     = 0;
    mem__lastchunk->prevrealsize = 0;
    mem__lastchunk->handle       = NULL;

    mem__pagesize = mem__heapsize;   /* reasonable approximation */
    mem__free = mem__heapsize;
  }

  return(TRUE);
#endif
}
