@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File:    Environment.s.OSCLI
@   Author:  Copyright © 1995 Sergio Monesi
@   Version: 1.00 (15 Jul 1995)
@            1.01 (28 Sep 2007) Moved to Environment module
@   Purpose: Veneer for OS_CLI


        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"




        .global Environment__OS_CLI
Environment__OS_CLI:

        MOV     ip, lr
        SWI     SWI_OS_CLI + XOS_Bit
        MOVVC   r0, #0
        MOV     pc, ip


