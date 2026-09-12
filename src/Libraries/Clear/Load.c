/*
 * File: Clear.Load.c
 * Author: Jason Howat, 16th December 1993
 *
 * Abstract: Load a Clear file into memory.
 *
 * This file is part of DeskLib, the general-purpose freeware C library for
 * RISC OS.  Please refer to the accompanying documentation for conditions
 * of use.
 */

#include <stdlib.h>

#include "DeskLib:Clear.h"
#include "DeskLib:File.h"
#include "DeskLib:Mem.h"


static clear_picture *Clear__AbortLoad(file_handle in, clear_picture *temp)
{
  if(in)
    File_Close(in);

  if(temp)
  {
    if(temp->bitmap)
      Mem_Free((void **)&temp->bitmap);
    if(temp->palette)
      Mem_Free((void **)&temp->palette);
    free(temp);
  }

  return NULL;
}

clear_picture *Clear_Load(const char *filename)
{
  file_handle   in = 0;
  clear_picture *temp = NULL;
  unsigned      creator_length = 1;
  unsigned      bitmap_size;
  int           word;
  unsigned      entry,
                colours;
  unsigned      i;
  palette_entry *palette;


  if((in = File_Open(filename, file_READ)) == 0)
    return NULL;

  while(File_ReadByte(in) > 0)
    creator_length++;

  if(file_lasterror)
    return Clear__AbortLoad(in, temp);

  if(File_Seek(in, 0))
    return Clear__AbortLoad(in, temp);

  if((temp = malloc(sizeof(clear_picture) + creator_length)) == NULL)
    return Clear__AbortLoad(in, temp);

  temp->creator = (char *)(int)&temp[1];
  temp->palette = NULL;
  temp->bitmap = NULL;

  if(File_ReadBytes(in, (char *)(int)temp->creator, creator_length))
    return Clear__AbortLoad(in, temp);

  temp->creatorversion = File_Read32(in);
  temp->width = File_Read32(in);
  temp->height = File_Read32(in);
  temp->bpp = File_Read32(in);

  if(file_lasterror)
    return Clear__AbortLoad(in, temp);

  if(temp->bpp > 8)
    bitmap_size = 3 * temp->width * temp->height;
  else
  {
    colours = 1 << temp->bpp;

    if(!Mem_Alloc((void **)&temp->palette, sizeof(palette_entry) * colours))
      return Clear__AbortLoad(in, temp);

    palette = temp->palette;
    for(entry = 0 ; entry < colours ; entry++)
    {
      word = 0;
      for(i = 8 ; i < 25 ; i += 8)
        word += File_ReadByte(in) << i;
      palette[entry].value = word;
    }

    if(file_lasterror)
      return Clear__AbortLoad(in, temp);

    bitmap_size = temp->width * temp->height;
  }

  if(!Mem_Alloc((void **)&temp->bitmap, bitmap_size))
    return Clear__AbortLoad(in, temp);

  if(File_ReadBytes(in, temp->bitmap, bitmap_size) || file_lasterror)
    return Clear__AbortLoad(in, temp);

  File_Close(in);
  return temp;
}
