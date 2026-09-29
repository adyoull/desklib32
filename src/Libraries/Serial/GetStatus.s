@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    Serial.s.GetStatus
@   Author  Copyright © 1993 Jason Howat
@   Version 1.00 (21 Nov 1993)
@   Purpose Read the status of the serial port.

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@ int Serial_GetStatus(void);

        
        .global Serial_GetStatus
Serial_GetStatus:
@
        STMFD   sp!, {lr}
        MOV     a1, #0
        MOV     a2, #0
        MVN     a3, #0
        SWI     SWI_OS_SerialOp + XOS_Bit
        MOV     a1, a2
        LDMFD   sp!, {pc}
@
