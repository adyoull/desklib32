/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Font.c.LoseAll
    Author:  Copyright © 1995 Julian Smith
    Version: 1.00 (08 Jul 1995)
    Purpose: Veneer for the RISC OS 3 Font_Paint SWI.
             If someone could convert this into assembler...
*/


#include "DeskLib:Font.h"
#include "DeskLib:SWI.h"

#ifndef SWI_Font_Paint
#define SWI_Font_Paint	0x40086
#endif

os_error	*Font_Paint3(
			font_handle		font,
			const char		*string,
			unsigned int		plottype,
			int			x,
			int			y,
			font_coorblock		*coorblock,
			font_transformation	*transformation,
			int			length
			)
{
return SWI( 8, 0, SWI_Font_Paint, font, string, plottype, x, y, coorblock, transformation, length);
}

