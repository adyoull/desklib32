/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Sprite.LoadFiles.c
    Author:  Sprite.LoadFile.c Copyright © 1994 Tim Browse
             modifications for multiple files Copyright © 1994 Lee Atkinson
    Version: 1.00 (25 Sep 1994)
    Purpose: Allocate a sprite area and load any number of files into it.
*/

#include <stdarg.h>
#include <stdlib.h>

#include "DeskLib:Core.h"
#include "DeskLib:File.h"
#include "DeskLib:Sprite.h"

extern sprite_area Sprite_LoadFiles(unsigned int nooffiles,...)
/* loads any number of sprite files into a sprite area (allocated by malloc()),
                                                  and returns a pointer to it */
{
 int          i,blksize=0;
 sprite_area  sprarea;
 va_list      argptr;
 va_start(argptr,nooffiles); /* inititialise arguments pointer */
 for (i=nooffiles;i;i--) blksize+=File_Size(va_arg(argptr,char *)); /* size of
                                                                    all files */
 blksize+=16; /* add room for extra header info & safety padding */
 if ((sprarea=(sprite_area)malloc(blksize))==NULL) /* allocate memory */
   {
    va_end(argptr); /* if allocation failed, restore stack */
    return NULL;    /*                            & return */
   }
 sprarea->areasize=blksize; /* initialise sprite area */
 sprarea->firstoffset=16;
 Sprite_InitArea(sprarea);
 va_start(argptr,nooffiles); /* initialise stack pointer */
 if (Sprite_Load(sprarea,va_arg(argptr,char *))) /* load 1st sprite file */
   {
    free(sprarea);  /* if load failed: deallocate;    */
    va_end(argptr); /*                 restore stack; */
    return NULL;    /*               & return         */
   }
 else
     {
      for (i=nooffiles-1;i;i--)
         {
          if (Sprite_Merge(sprarea,va_arg(argptr,char *))) /* merge the rest */
            {
             free(sprarea);  /* if a merge failed: deallocate;    */
             va_end(argptr); /*                    restore stack; */
             return NULL;    /*                 &  return         */
            }
         }
     }
 va_end(argptr); /* restore stack */
 return sprarea; /* return */
}
