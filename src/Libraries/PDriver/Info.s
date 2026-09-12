;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    PDriver.Info.s
;   Author:  Copyright © 1993 Jason Howat
;   Version: 1.00 (06 Jul 1993)
;   Purpose: Get information on the printer driver.

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; os_error *PDriver_Info(printer_info *block);

        PREAMBLE
        STARTCODE PDriver_Info
;
        STMFD   sp!, {r4,r5,r6,r7,r8,lr}
        MOV     r8, a1
        SWI     SWI_PDriver_Info + XOS_Bit
        STMVCIA r8, {r0,r1,r2,r3,r4,r5,r6,r7}
        MOVVC   a1, #0
        LDMFD   sp!, {r4,r5,r6,r7,r8,pc}
;
        END
