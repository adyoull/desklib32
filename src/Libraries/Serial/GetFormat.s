;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    Serial.s.GetFormat
;   Author:  Copyright © 1993 Jason Howat
;   Version: 1.00 (21 Nov 1993)
;   Purpose: Read the data format for the encoding of characters sent and
;            received on the serial line.

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; int Serial_GetFormat(void);

        PREAMBLE
        STARTCODE Serial_GetFormat
;
        STMFD   sp!, {lr}
        MOV     a1, #1
        MVN     a2, #0
        SWI     SWI_OS_SerialOp + XOS_Bit
        MOV     a1, a2
        LDMFD   sp!, {pc}
;
        END
