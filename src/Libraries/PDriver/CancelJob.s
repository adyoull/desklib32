;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    PDriver.CancelJob.s
;   Author:  Copyright © 1993 Jason Howat
;   Version: 1.00 (07 Jul 1993)
;   Purpose: Stop the print job from printing.

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; os_error *PDriver_CancelJob(print_job job);

        PREAMBLE
        STARTCODE PDriver_CancelJob
;
        STMFD   sp!, {lr}
        SWI     SWI_PDriver_CancelJob + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
;
        END
