;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    Misc.PrintrName.s
;   Author:  Copyright © 1993 Jason Williams
;   Version: 1.00 (22 May 1993)
;   Purpose: Calls PDriver_Info, and returns the Printer name, or 0 if no
;            printer is installed. (only tested on RO3.10)

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

        PREAMBLE
        STARTCODE PDriver_PrinterName
;
        STMFD sp!, {r1-r6, lr}
        SWI SWI_PDriver_Info + XOS_Bit
        MOVVS r0, #0
        MOVVC r0, r4
        LDMFD sp!, {r1-r6, pc}
;
        END
