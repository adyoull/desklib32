;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    KernelSWIs.s.OSByte
;   Author:  Copyright © 1993 Jason Howat
;   Version: 1.00 (04 Dec 1993)
;   Purpose: Generic veneer to the OS_Byte SWI.

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; os_error *OS_Byte(osbyte_number reason, int r1in, int r2in,
;                                         int *r1out, int *r2out);

        PREAMBLE
        STARTCODE OS_Byte
;
        MOV     ip, sp
        STMFD   sp!, {lr}
        SWI     SWI_OS_Byte + XOS_Bit
        MOVVC   r0, #0
        CMP     r3, #0
        STRNE   r1, [r3]
        LDR     r3, [ip]
        CMP     r3, #0
        STRNE   r2, [r3]
        LDMFD   sp!, {pc}
;
        END
