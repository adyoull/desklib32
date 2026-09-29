;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    Serial.s.PutByte
;   Author:  Copyright © 1993 Jason Howat
;   Version: 1.00 (21 Nov 1993)
;   Purpose: Sends a byte to the serial port.

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; BOOL Serial_PutByte(int c);

        PREAMBLE
        STARTCODE Serial_PutByte
;
        STMFD   sp!, {lr}
        MOV     a2, a1
        MOV     a1, #3
        SWI     SWI_OS_SerialOp + XOS_Bit
        MOVCC   a1, #0
        MOVCS   a1, #1
        MOVVS   a1, #1
        LDMFD   sp!, {pc}
;
        END
