@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    Serial.s.SetStatus
@   Author  Copyright © 1993 Jason Howat
@   Version 1.00 (21 Nov 1993)
@   Purpose Configure state of serial system.

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@ os_error *Serial_SetStatus(int eor_mask, int and_mask);

        
        .global Serial_SetStatus
Serial_SetStatus:
@
        STMFD   sp!, {lr}
        MOV     a3, a2
        MOV     a2, a1
        MOV     a1, #0
        SWI     SWI_OS_SerialOp + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
@
