/*
 * File: Clear.Free.c
 * Author: Jason Howat, 16th December 1993
 *
 * Abstract: Release all memory used for a Clear image.
 *
 * This file is part of DeskLib, the general-purpose freeware C library for
 * RISC OS.  Please refer to the accompanying documentation for conditions
 * of use.
 */


#include <stdlib.h>

#include "DeskLib:Clear.h"
#include "DeskLib:Mem.h"


void Clear_Free(clear_picture *picture)
{
  if(picture->bitmap)
    Mem_Free((void *)&picture->bitmap);
  if(picture->palette)
    Mem_Free((void *)&picture->palette);
  free(picture);
}
