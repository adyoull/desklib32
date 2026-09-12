/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Dialog2.EnsureWind.c
    Author:  Copyright © 1994 Julian Smith
    Version: 1.03 (13 Mar 1995)
    Purpose: Dialogue box handling
*/


#include "DeskLib:Window.h"
#include "DeskLib:Dialog2.h"


void	Dialog2_EnsureWindowHandle( dialog2_block *dialog2)
{
if ( !dialog2->window)
	dialog2->window = Window_Create(
		dialog2->templatename, dialog2->flags.data.maxtitlesize
		);
}


