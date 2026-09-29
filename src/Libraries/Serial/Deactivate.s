@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    Serial.s.Deactivate
@   Author  Copyright © 1993 Jason Howat
@   Version 1.00 (21 Nov 1993)
@   Purpose Disable the serial port.

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@ os_error *Serial_Deactivate(void);

        
        .global Serial_Deactivate
Serial_Deactivate:
@
        STMFD   sp!, {lr}
        MOV     a1, #2
        MOV     a2, #0
        SWI     SWI_OS_Byte + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
@
