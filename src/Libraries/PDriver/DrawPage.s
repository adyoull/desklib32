;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    PDriver.DrawPage.s
;   Author:  Copyright © 1993 Jason Howat
;   Version: 1.00 (07 Jul 1993)
;   Purpose: Initiates printing after rectangles have been specified.

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; os_error *PDriver_DrawPage(int copies, wimp_rect *rectangle_to_print, int sequence_no, const char *page,
;                            int *finished, int *rectangle_id);

        PREAMBLE
        STARTCODE PDriver_DrawPage
;
        MOV     ip, sp
        STMFD   sp!, {lr}
        SWI     SWI_PDriver_DrawPage + XOS_Bit
        LDMVSFD sp!, {pc}         ; Error - return directly

        LDR     a2, [ip, #0]
        CMP     a2, #0
        STRNE   a1, [a2, #0]
        LDR     a2, [ip, #4]
        CMP     a2, #0
        STRNE   a3, [a2, #0]
        MOV     a1, #0
        LDMFD   sp!, {pc}
;
        END
