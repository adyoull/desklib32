;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    Serial.s.Activate
;   Author:  Copyright © 1993 Jason Howat
;   Version: 1.00 (21 Nov 1993)
;   Purpose: Enable the serial port.

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; os_error *Serial_Activate(void);

        PREAMBLE
        STARTCODE Serial_Activate
;
        STMFD   sp!, {lr}
        MOV     a1, #2
        MOV     a2, #2
        SWI     SWI_OS_Byte + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
;
        END
