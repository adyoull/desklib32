@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    Misc.ConvertDAT.s
@   Author  Copyright © 1993 Jason Howat
@   Version 1.00 (01 August 1993)
@   Purpose Convert given time to text using format string.

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

        
        .global Time_ConvertDateAndTime
Time_ConvertDateAndTime:
@
        STMFD   sp!, {lr}
        SWI     SWI_OS_ConvertDateAndTime + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
@
