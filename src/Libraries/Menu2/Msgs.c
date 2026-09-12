/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Menu2.Msgs.c
    Author:  Copyright © 1995 Julian Smith
    Version: 1.00 (12 Jul 1995)
    Purpose: Creating menus using message tags for menu title and
             specification
*/

#include "DeskLib:Msgs.h"
#include "DeskLib:Menu2.h"
#include "DeskLib:Str.h"


menu2_handle	Menu2_CreateFromMsgs(
			const char		*titletag,
			const char		*spectag,
			menu2_makefn	makefn,
			menu2_flagsfn	flagsfn,
			menu2_subfn	subfn,
			menu2_selectfn	selectfn,
			void		*reference
			)
{
char	specbuffer[ 256];
char	titlebuffer[ 16];
char	*spec, *title;

Msgs_Lookup( titletag, titlebuffer, 15);
Msgs_Lookup( spectag, specbuffer, 255);
spec = strdup( specbuffer);
title = strdup( titlebuffer);

return Menu2_Create( title, spec, makefn, flagsfn, subfn, selectfn, reference);
}

