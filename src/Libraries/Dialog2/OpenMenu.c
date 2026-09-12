/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Dialog2.OpenMenu.c
    Author:  Copyright © 1994 Julian Smith
    Version: 1.03 (13 Mar 1995)
    Purpose: Dialogue box handling
*/


#include "DeskLib:WimpSWIs.h"


#include "Defs.h"



BOOL	Dialog2_OpenDialogMenu( dialog2_block *dialog2, window_openpos openpos)
{
wimp_point	pos;

if ( !dialog2)	return ERROR;

Dialog2__CommonOpenMenuCode( dialog2);	/* Close any previous menuleaf etc.	*/

dialog2->flags.data.type = dialog2_type_MENU;
Dialog2__CommonOpenCode( dialog2);

Dialog2_Window_GetWindowOpenPos( &pos, openpos, dialog2->window);
Wimp_CreateMenu( (menu_block *) -1, -1, -1);
Wimp_CreateMenu( (menu_block *) dialog2->window, pos.x, pos.y);

return NOERROR;
}
