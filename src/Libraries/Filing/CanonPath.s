;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    Filing.s.CanonPath
;   Author:  Copyright © 1994, 1995 Sergio Monesi
;   Version: 1.00 (13 Jul 1995)
;   Purpose: Converts a pathname to a canonicalised path

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; os_error *Filing_CanonicalisePath(char *pathname, char *buffer,
;                                   int size, int *spare);

        PREAMBLE
        STARTCODE Filing_CanonicalisePath
;
        STMFD   sp!, {r4-r5,lr}
        MOV     ip, r3
        MOV     r5, r2
        MOV     r4, #0
        MOV     r3, #0
        MOV     r2, r1
        MOV     r1, r0
        MOV     r0, #37
        SWI     SWI_OS_FSControl + XOS_Bit
        LDMVSFD sp!, {r4-r5,pc}
        MOV     r0,#0
        TEQ     ip, #0
        STRNE   r5, [ip]
        LDMFD   sp!, {r4-r5,pc}
;
        END
