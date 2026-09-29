@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    Serial.s.SendBreak
@   Author  Copyright © 1993 Jason Howat
@   Version 1.00 (21 Nov 1993)
@   Purpose Transmit a break on the serial port.

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@ os_error *Serial_SendBreak(int length);

        
        .global Serial_SendBreak
Serial_SendBreak:
@
        STMFD   sp!, {lr}
        MOV     a2, a1
        MOV     a1, #2
        SWI     SWI_OS_SerialOp + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
@
