;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    Serial.s.SetFormat
;   Author:  Copyright © 1993 Jason Howat
;   Version: 1.00 (21 Nov 1993)
;   Purpose: Set the format for data to be transmitted and received over
;            the serial line.

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; os_error *Serial_SetFormat(int format);

        PREAMBLE
        STARTCODE Serial_SetFormat
;
        STMFD   sp!, {lr}
        MOV     a2, a1
        MOV     a1, #1
        SWI     SWI_OS_SerialOp + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
;
        END
