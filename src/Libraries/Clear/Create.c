/*
 * File: Clear.Create.c
 * Author: Jason Howat, 16th December 1993
 *
 * Abstract: Allocate memory and initialise it for a Clear image.
 *
 * This file is part of DeskLib, the general-purpose freeware C library for
 * RISC OS.  Please refer to the accompanying documentation for conditions
 * of use.
 */

#include <stdlib.h>
#include "DeskLib:Clear.h"
#include "DeskLib:Mem.h"

#include "DeskLib:Clear.h"
#include "ClearDefs.h"




clear_picture *Clear_Create(unsigned width, unsigned height, unsigned bpp)
{
  clear_picture *temp;
  unsigned      bitmap_size;


  if((temp = malloc(sizeof(clear_picture))) == NULL)
    return NULL;

  temp->creator = clear__creator;
  temp->creatorversion = clear__creatorversion;
  temp->width = width;
  temp->height = height;
  temp->bpp = bpp;
  if(bpp > 8)
  {
    temp->palette = NULL;
    bitmap_size = 3 * width * height;
  }
  else
  {
    if(!Mem_Alloc((void **)&temp->palette, sizeof(palette_entry) * (1 << bpp)))
    {
      free(temp);
      return NULL;
    }

    bitmap_size = width * height;
  }

  if(!Mem_Alloc((void **)&temp->bitmap, bitmap_size))
  {
    if(temp->palette)
      Mem_Free((void **)temp->palette);
    free(temp);
  }

  return temp;
}
