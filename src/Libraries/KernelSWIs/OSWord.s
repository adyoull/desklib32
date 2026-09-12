;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    KernelSWIs.s.OSWord
;   Author:  Copyright © 1993 Jason Howat
;   Version: 1.00 (04 Dec 1993)
;   Purpose: Generic veneer to the OS_Word SWI.

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; os_error *OS_Word(osword_number reason, void *param_block);

        PREAMBLE
        STARTCODE OS_Word
;
        STMFD   sp!, {lr}
        SWI     SWI_OS_Word + XOS_Bit
        MOVVC   r0, #0
        LDMFD   sp!, {pc}
;
        END
