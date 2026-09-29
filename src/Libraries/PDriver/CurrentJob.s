;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    PDriver.CurrentJob.s
;   Author:  Copyright © 1993 Jason Howat
;   Version: 1.00 (06 Jul 1993)
;   Purpose: Returns the handle of the current job.

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; os_error *PDriver_CurrentJob(print_job *current);

        PREAMBLE
        STARTCODE PDriver_CurrentJob
;
        STMFD   sp!, {lr}
        MOV     a2, a1
        SWI     SWI_PDriver_CurrentJob + XOS_Bit
        STRVC   r0, [a2,#0]
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
;
        END
