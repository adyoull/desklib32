;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    PDriver.CancelJobW.s
;   Author:  Copyright © 1993 Jason Howat
;   Version: 1.00 (07 Jul 1993)
;   Purpose: Cancels a print job - future attempts to output to it
;            generate an error.

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; os_error *PDriver_CancelJobWithError(print_job job, os_error *error);

        PREAMBLE
        STARTCODE PDriver_CancelJobWithError
;
        STMFD   sp!, {lr}
        SWI     SWI_PDriver_CancelJobWithError + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
;
        END
