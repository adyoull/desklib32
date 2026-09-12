;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    PDriver.EndJob.s
;   Author:  Copyright © 1993 Jason Howat
;   Version: 1.00 (06 Jul 1993)
;   Purpose: End a print job normally.

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; os_error *PDriver_EndJob(print_job job);

        PREAMBLE
        STARTCODE PDriver_EndJob
;
        STMFD   sp!, {lr}
        SWI     SWI_PDriver_EndJob + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
;
        END
