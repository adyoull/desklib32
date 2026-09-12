/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Resource.LoadSprite.c
    Author:  Copyright © 1994 Lenny
    Version: 0.01 (18 Nov 1994)
    Purpose: Creates a user sprite area, and loads the application's
             resource sprite file ("Sprites") into this area.
    History: 0.01 (18 Nov 94) : Added 'Resource_LoadSprites()'

*/

/* --- LOAD HEADERS ------------------------------------------------------ */

/* --- DeskLib ----------------------------------------------------------- */
#include "DeskLib:Error.h"             /* Error despatcher                 */
#include "DeskLib:File.h"              /* Low level file handling          */
#include "DeskLib:Resource.h"          /* Handles finding resource files   */
#include "DeskLib:Screen.h"            /* Getting screen size info, etc    */
#include "DeskLib:Sprite.h"            /* Sprite handling                  */

/* --- CLib -------------------------------------------------------------- */
#include <stdlib.h>      /*  Standard library routines                     */
#include <stdio.h>       /*  General I/O routines                          */

/* === FUNCTION DEFINITIONS ============================================== */

/*  Global variable for a pointer to the resource user sprite area.
 *  Defaults to the Wimp sprite pool.
 */


sprite_area resource_sprites = (sprite_area) 1;


extern void Resource_LoadSprites(void)
{
  char                  filename[256];
  int                   filesize;

  if (resource_sprites != (sprite_area) 1)
    return;   /* The resources have already been loaded */

  Screen_CacheModeInfo();
  if (screen_eig.y == 1) {
    sprintf(filename, "%sSprites22", resource_pathname);
    if (File_Exists(filename) == FALSE) {
      sprintf(filename, "%sSprites", resource_pathname);
    }
  } else {
    sprintf(filename, "%sSprites", resource_pathname);
  }

  filesize = File_Size(filename);
  if (filesize == 0) {
    Error_ReportFatal(1, "'Sprites' resource file not found");
  }

  resource_sprites = malloc(filesize + 16);
  if (resource_sprites == NULL)
  {
    Error_ReportFatal(1, "Couldn't allocate memory to load 'Sprites'.");
  }

  resource_sprites->areasize = filesize + 16;
  resource_sprites->firstoffset = 16;
  Sprite_InitArea(resource_sprites);
  Sprite_Load(resource_sprites, filename);

}

/***************************************************************************/
