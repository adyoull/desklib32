@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    PDriver.AbortJob.s
@   Author  Copyright © 1993 Jason Howat
@   Version 1.00 (06 Jul 1993)
@   Purpose End a print job without any further output.

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@ os_error *PDriver_AbortJob(print_job job);

        
        .global PDriver_AbortJob
PDriver_AbortJob:
@
        STMFD   sp!, {lr}
        SWI     SWI_PDriver_AbortJob + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
@
