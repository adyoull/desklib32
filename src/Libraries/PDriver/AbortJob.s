;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    PDriver.AbortJob.s
;   Author:  Copyright © 1993 Jason Howat
;   Version: 1.00 (06 Jul 1993)
;   Purpose: End a print job without any further output.

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; os_error *PDriver_AbortJob(print_job job);

        PREAMBLE
        STARTCODE PDriver_AbortJob
;
        STMFD   sp!, {lr}
        SWI     SWI_PDriver_AbortJob + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
;
        END
