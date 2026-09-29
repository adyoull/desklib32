;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    PDriver.EnumerateJ.s
;   Author:  Copyright © 1993 Jason Howat
;   Version: 1.00 (07 Jul 1993)
;   Purpose: List existing print jobs.

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; os_error *PDriver_EnumerateJobs(print_job *handle);

        PREAMBLE
        STARTCODE PDriver_EnumerateJobs
;
        STMFD   sp!, {lr}
        MOV     a2, a1
        LDR     a1, [a1, #0]
        SWI     SWI_PDriver_EnumerateJobs + XOS_Bit
        STRVC   a1, [a2, #0]
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
;
        END
