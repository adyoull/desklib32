@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    KernelSWIs.s.ConvTTUTCO
@   Author  Copyright © 2003 Antony Sidwell
@   Version 1.00 (15 Jan 2003)
@   Purpose Converts five-byte time block to UTC  time ordinals

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@ os_error *Time_ConvertTimeToUTCOrdinals(char *fivebyteblock,
@                                         time_ordinals *ord);

        
        .global Time_ConvertTimeToUTCOrdinals
Time_ConvertTimeToUTCOrdinals:
@              
        STMFD   sp!, {lr}
        MOV     r2, r1
        MOV     r1, r0
        SWI     SWI_Territory_ConvertTimeToUTCOrdinals + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
@
