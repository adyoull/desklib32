;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    PDriver.SelectIllu.s
;   Author:  Copyright © 1993 Jason Howat
;   Version: 1.00 (07 Jul 1993)
;   Purpose: Makes the given print job the current one, and treats
;            it as an illustration.

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; os_error *PDriver_SelectIllustration(print_job new, const char *title, print_job *old);

        PREAMBLE
        STARTCODE PDriver_SelectIllustration
;
        STMFD   sp!, {lr}
        SWI     SWI_PDriver_SelectIllustration + XOS_Bit
        LDMVSFD sp!, {pc}
        CMP     a3, #0
        STRNE   r0, [a3, #0]
        MOV     a1, #0
        LDMFD   sp!, {pc}
;
        END
