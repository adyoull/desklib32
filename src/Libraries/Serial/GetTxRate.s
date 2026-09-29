@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    Serial.s.GetTxRate
@   Author  Copyright © 1993 Jason Howat
@   Version 1.00 (21 Nov 1993)
@   Purpose Read the baud rate of data transmitted on the serial line.

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@ serial_rate Serial_GetTxRate(void);

        
        .global Serial_GetTxRate
Serial_GetTxRate:
@
        STMFD   sp!, {lr}
        MOV     a1, #6
        MVN     a2, #0
        SWI     SWI_OS_SerialOp + XOS_Bit
        MOV     a1, a2
        LDMFD   sp!, {pc}
@
