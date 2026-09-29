@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    PDriver.SelectJob.s
@   Author  Copyright © 1993 Jason Howat
@   Version 1.00 (06 Jul 1993)
@   Purpose Make a given print job the current one.

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@ os_error *PDriver_SelectJob(print_job new, const char *title, print_job *old);

        
        .global PDriver_SelectJob
PDriver_SelectJob:
@
        STMFD   sp!, {lr}
        SWI     SWI_PDriver_SelectJob + XOS_Bit
        LDMVSFD sp!, {pc}
        CMP     a3, #0
        STRNE   r0, [a3,#0]
        MOV     a1, #0
        LDMFD   sp!, {pc}
@
