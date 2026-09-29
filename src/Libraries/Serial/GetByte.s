;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    Serial.s.GetByte
;   Author:  Copyright © 1993 Jason Howat
;   Version: 1.00 (21 Nov 1993)
;   Purpose: Read a byte from the serial port.

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; int Serial_GetByte(void);

        PREAMBLE
        STARTCODE Serial_GetByte
;
        STMFD   sp!, {lr}
        MOV     a1, #4
        SWI     SWI_OS_SerialOp + XOS_Bit
        MOVCC   a1, a2
        MVNCS   a1, #0
        MVNVS   a1, #0
        LDMFD   sp!, {pc}
;
        END
