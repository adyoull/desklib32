@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    KernelSWIs.s.ConvertTTO
@   Author  Copyright © 2003 Antony Sidwell
@   Version 1.00 (15 Jan 2003)
@   Purpose Converts five-byte time block to local time ordinals

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@ os_error *Time_ConvertTimeToOrdinals(char *fivebyteblock, time_ordinals *ord);

        
        .global Time_ConvertTimeToOrdinals
Time_ConvertTimeToOrdinals:
@              
        STMFD   sp!, {lr}
        MOV     r2, r1
        MOV     r1, r0
        MOV     r0, #-1    @ we want time in the current territory
        SWI     SWI_Territory_ConvertTimeToOrdinals + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
@
