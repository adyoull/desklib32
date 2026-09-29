/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Dialog2.Create.c
    Author:  Copyright © 1994 Julian Smith
    Version: 1.03 (13 Mar 1995)
    Purpose: Dialogue box handling
*/


#include <stdlib.h>

#include "DeskLib:Error.h"
#include "DeskLib:Template.h"
#include "DeskLib:Dialog2.h"


dialog2_block	*Dialog2_CreateDialogBlock(
	const char			*templatename,
	icon_handle		okbutton,
	icon_handle		cancelbutton,
	dialog2_openfn		openfn,
	dialog2_okfn		okfn,
	void			*reference
	)
{
dialog2_block	*dialog2;

dialog2 = malloc( sizeof( dialog2_block));
if ( !dialog2)	{
	Error_OutOfMemory( FALSE, "dialog2 box");
	return NULL;
	}

 dialog2->window				= (window_handle)0;
dialog2->templatename		= templatename;
dialog2->reference			= reference;
dialog2->openfn				= openfn;
dialog2->okfn				= okfn;

dialog2->flags.value			= 0;
dialog2->flags.data.type		= dialog2_type_CLOSED;
dialog2->flags.data.keepwindow		= FALSE;
dialog2->flags.data.notifyclose		= FALSE;
dialog2->flags.data.okbutton		= okbutton;
dialog2->flags.data.cancelbutton	= cancelbutton;
dialog2->flags.data.maxtitlesize	= template_TITLEMIN;

return dialog2;
}


