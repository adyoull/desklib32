/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Dialog2.OpenStatic.c
    Author:  Copyright © 1994 Julian Smith
    Version: 1.03 (13 Mar 1995)
    Purpose: Dialogue box handling
*/


#include "Defs.h"



BOOL	Dialog2_OpenDialogStatic( dialog2_block *dialog2, window_openpos openpos)
{
if ( !dialog2)	return ERROR;

if ( dialog2->flags.data.type == dialog2_type_MENULEAF)
	Dialog2__CloseDialogInternal( dialog2, FALSE, FALSE/*dontdeletewindow*/);

else if ( dialog2->flags.data.type == dialog2_type_STATIC)
	Dialog2__CloseDialogInternal( dialog2, FALSE/*dontclosemenu*/, FALSE/*dontdeletewindow*/);
	/* This dialog2 is already open as a static dialog, so close it,	*/
	/* but keep its window handle and don't close any existing menu.	*/

else if ( dialog2->flags.data.type == dialog2_type_MENU)
	Dialog2__CloseDialogInternal( dialog2, TRUE/*close exisiting menu*/, FALSE);
	/* This dialog2 is already open as a menu-window, so close it, 	*/
	/* deleting the menu. This won't be that good in RO2 - it will 	*/
	/* close the existing menu sometimes.				*/



dialog2->flags.data.type	= dialog2_type_STATIC;
Dialog2__CommonOpenCode( dialog2);

Window_Show( dialog2->window, openpos);

return NOERROR;
}

