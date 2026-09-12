/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Sprite.MemSizeIcn.c
    Author:  Copyright © 1993 Tom Kirby-Green
    Version: 1.00 (12 Jul 1993)
    Purpose: Calculate the size in bytes of a sprite with the given parameters
             if it were to exactly fill the given WIMP icon. Also returns
             the width and height of the icon.
*/


#include "DeskLib:Sprite.h"
#include "DeskLib:SWI.h"


extern int Sprite_IconMemorySize(window_block *pWinBlock, icon_handle icon,
                                 screen_modeval mode,
                                 spritemem_flags flags,
                                 wimp_point *dimensions)
{
  wimp_rect  *iconbox;
  wimp_point eig, size;

  /* Extract icon's bounding box */
  iconbox = &(((icon_block*)(pWinBlock+1))[icon].workarearect);

  /* Get scaling factors */
  SWI(2, 3, SWI_OS_ReadModeVariable,
                mode, SCREEN_VAR_XEigFactor,
      /* TO */  NULL, NULL, &eig.x );

  SWI(2, 3, SWI_OS_ReadModeVariable,
                mode, SCREEN_VAR_YEigFactor,                   
      /* TO */  NULL, NULL, &eig.y );

  /* Convert OS units to pixels */
  size.x  = ( iconbox->max.x - iconbox->min.x ) >> eig.x;
  size.y  = ( iconbox->max.y - iconbox->min.y ) >> eig.y;

  if (dimensions != NULL)          /* If user wants size returned, return it */
    *dimensions  = size;

  return(Sprite_MemorySize(size.x, size.y, mode, flags));
}
