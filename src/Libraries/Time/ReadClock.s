@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    Time.s.ReadClock
@   Author  Copyright © 2003 Antony Sidwell
@   Version 1.00 (15 Jan 2003)
@   Purpose Reads the 5 byte time block.

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@ os_error *Time_ReadClock(char *fivebyteblock);

        
        .global Time_ReadClock
Time_ReadClock:
@
        STMFD   sp!, {lr}
        MOV     r1, r0
        MOV     r0, #3
        STRB    r0, [r1, #0]
        MOV     r0, #14
        SWI     SWI_OS_Word + XOS_Bit
        MOVVC   r0, #0
        LDMFD   sp!, {pc}
@
