/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Sprite.MemSize.c
    Author:  Copyright © 1993 Tom Kirby-Green, Jason Howat
    Version: 1.01 (02 Sep 1995)
    Purpose: Calculate the size in bytes of a sprite with the given parameters
    Mods:    02 Sep 1995 JH Uses mode variable Log2BPC (bytes per chr) instead
                            of Log2BPP (bits per pixel), to work properly with
                            'double pixel' modes such as mode 4.
*/


#include "DeskLib:Sprite.h"
#include "DeskLib:SWI.h"


/* Macro to round a number of BITS up to a multiple of words */
#define ROUND32(value) (((value) + 31) & ~31)


extern unsigned int Sprite_MemorySize(unsigned int width,
                                      unsigned int height,
                                      screen_modeval mode,
                                      spritemem_flags flags)
{
  unsigned int log2bpp;      /* Log base 2 bits per pixel, i.e. 2^log2bpp */

  SWI(2, 3, SWI_OS_ReadModeVariable,             /* Get Log2BPC for mode */
                 mode, SCREEN_VAR_Log2BPC,       /* (used to be Log2BPP  */
       /* TO */  NULL, NULL, &log2bpp );

  return Sprite_MemorySizeBpp(width, height, 1 << log2bpp, flags);
}


extern unsigned int Sprite_MemorySizeBpp(unsigned int width,
                                         unsigned int height,
                                         unsigned int bpp,
                                         spritemem_flags flags)
{
  int bit_strlen;        /* Bit string length                         */
  size_t size;           /* Memory requirements                       */

  bit_strlen = ROUND32(width * bpp); /* Initial bit string length,    *
                                      * rounded up to nearest word    */

  bit_strlen >>= 3;                  /* Convert bits to bytes (/= 8)  */
  size = bit_strlen * height;        /* Multiply by number of columns */

  if (flags & sprite_HASMASK)
  {
    if (flags & sprite_DEEPFORMAT)
    {
      /* For deep format sprites, the mask uses the same as a
         1 bpp sprite of the same dimensions as the image */
      size += ROUND32(width) * height;
    }
    else
    {
      /* For old format sprites, the mask is a chunk the same size as the data.  */
      size *= 2;
    }
  }

  size += sizeof( sprite_header);    /* Add on 44 bytes for header    */

  if ((flags & sprite_HASPAL) && bpp <= 8)  /* Add on size of palette        */
  {
    static char colours[] = { 0, 2, 4, 0, 16, 0, 0, 0, 64 };

    size += colours[bpp] * ( sizeof( palette_entry ) * 2);
  }

  return(size);
}
