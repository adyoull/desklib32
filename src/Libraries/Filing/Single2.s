;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    Filing.s.Single2
;   Author:  Copyright © 1995 Sergio Monesi
;   Version: 1.00 (28 Jul 1995)
;   Purpose: Reads a single directory entry

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; os_error *Filing_SingleDirEntry2(char *dirname, void *buf,
;                                  int size, char *filename);

        PREAMBLE
        STARTCODE Filing_SingleDirEntry2
;
        STMFD   sp!, {r4-r6,lr}
        MOV     r6, r3
        MOV     r5, r2
        MOV     r4, #0
        MOV     r2, r1
        MOV     r1, r0
        MOV     r0, #10

loop
        MOV     r3, #256
        SWI     SWI_OS_GBPB + XOS_Bit
        LDMVSFD sp!, {r4-r6,pc}

        TEQ     r3, #1
        BEQ     endok
        TEQ     r4, #0
        BGE     loop

        MOV     r0, #0                 ;
        STR     r0, [r2, #16]          ; objtype = 0
        LDMFD   sp!, {r4-r6,pc}

endok
        MOV     r0, #0
        LDMFD   sp!, {r4-r6,pc}
;
        END
