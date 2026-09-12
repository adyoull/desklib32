/*
    ####             #    #     # #
    #   #            #    #       #          The FreeWare C library for
    #   #  ##   ###  #  # #     # ###             RISC OS machines
    #   # #  # #     # #  #     # #  #   ___________________________________
    #   # ####  ###  ##   #     # #  #
    #   # #        # # #  #     # #  #    Please refer to the accompanying
    ####   ### ####  #  # ##### # ###    documentation for conditions of use
    ________________________________________________________________________

    File:    Event.taskname.c
    Author:  Copyright © 1992, 1993, 1994 John Winters
             (mucked about by Jason Williams and Tim Browse)
             (and by Julian Smith)
    Version: 1.00 (05 Jun 1995)
    Purpose: Defines 'event_taskname'.


*/

#include "DeskLib:Event.h"

char            event_taskname[40] = "";
