/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Sprite.LoadFile.c
    Author:  Copyright © 1994 Tim Browse
    Version: 1.00 (02 Mar 1994)
    Purpose: Allocate a sprite area and load a file into it.
*/

#include <stdlib.h>

#include "DeskLib:Core.h"
#include "DeskLib:File.h"
#include "DeskLib:Sprite.h"

extern sprite_area Sprite_LoadFile(const char *filename)
/*
 * This loads the given sprite file into a sprite area, and returns a pointer to
 * it.  The memory is automatically allocated with malloc().
 */
{
  int          blksize;
  sprite_area  sprarea;

  /*
   * To load file, allocate and initialise sprite area block, and call SWI to
   *  load in file.
   */

  /* Find out how big file is */
  blksize = File_Size(filename);

  /* Add a bit of room for extra header info & safety padding */
  blksize += 16;

  /* Allocate memory for this block */
  sprarea = (sprite_area) malloc(blksize);
  if (sprarea == NULL)
    return NULL;

  /* Initialise sprite area */
  sprarea->areasize    = blksize;
  sprarea->firstoffset = 16;

  Sprite_InitArea(sprarea);

  /* Try to load sprite file */
  if (Sprite_Load(sprarea, filename))
  {
    /* Sprite load failed; deallocate sprite area and inform caller */
    free(sprarea);
    return NULL;
  }

  return sprarea;
}

