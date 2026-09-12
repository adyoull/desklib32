/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Template.UseSprite.c
    Author:  Copyright © 1994 Tim Browse
    Version: 1.00 (02 Mar 1994)
    Purpose: Set the sprite area to be used for patching window templates.
*/


#include "TempDefs.h"

extern void Template_UseSpriteArea(sprite_area area)
{
  /* They don't come much simpler! */
  __template_spritearea = area;
}

