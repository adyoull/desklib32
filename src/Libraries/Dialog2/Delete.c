/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Dialog2.Delete.c
    Author:  Copyright © 1994 Julian Smith
    Version: 1.03 (13 Mar 1995)
    Purpose: Dialogue box handling
*/


#include <stdlib.h>

#include "DeskLib:Error.h"
#include "DeskLib:Dialog2.h"


BOOL	Dialog2_DeleteDialog( dialog2_block *dialog2)
{
if ( !dialog2)	return ERROR;

dialog2->flags.data.keepwindow = FALSE;
Dialog2_CloseDialog( dialog2);
free( dialog2);
return NOERROR;
}



