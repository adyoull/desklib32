;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    PDriver.PageSize.s
;   Author:  Copyright © 1993 Jason Howat
;   Version: 1.00 (06 Jul 1993)
;   Purpose: Read the size of the paper and how big the printable area is.

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; os_error *PDriver_PageSize(wimp_point *size, wimp_rect *printable);

        PREAMBLE
        STARTCODE PDriver_PageSize
;
        STMFD   sp!, {r4,r5,r6,r7,r8,lr}
        MOV     r7, a1
        MOV     r8, a2
        SWI     SWI_PDriver_PageSize + XOS_Bit
        LDMVSFD sp!, {r4,r5,r6,r7,r8,pc}
        CMP     r7, #0
        STMNEIA r7, {r1,r2}
        CMP     r8, #0
        STMNEIA r8, {r3,r4,r5,r6}
        MOV     a1, #0
        LDMFD   sp!, {r4,r5,r6,r7,r8,pc}
;
        END
