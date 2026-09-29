/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    WAssert.h
    Author:  Copyright © 1993 Mark H. Wilkinson
    Version: 1.00 (05 Sep 1993)
             1.01 (25 Oct 2007) Deprecated
    Purpose: Wimp version of ANSI assert() system, based on RISCOS_Lib
             version presented in Archive magazine 6.12.53 by Remo Biagioni.
*/

/*
   NOTE: This module is deprecated. Use the Debug Module instead
*/


#include <stdlib.h>
#include <string.h>

#include "DeskLib:Event.h"
#include "DeskLib:WimpSWIs.h"
#include "DeskLib:WAssert.h"


void __wassert(const char *message)
{
	os_error error;
	error.errnum = 0;
	strncpy(error.errmess, message, sizeof(error.errmess));
	Wimp_ReportError(&error, 1, event_taskname);
	exit(1);
}
