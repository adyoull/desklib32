@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    Module.s.06Claim
@   Author  Copyright © 1993 Jason Howat
@   Version 1.00 (23 Nov 1993)
@   Purpose Allocate a block of the RMA.

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@ os_error *Module_Claim(unsigned size, void **block);

        
        .global Module_Claim
Module_Claim:
@
        STMFD   sp!, {v1,lr}
        MOV     a4, a1
        MOV     a1, #6
        MOV     v1, a2
        SWI     SWI_OS_Module + XOS_Bit
        MOVVC   a1, #0
        STRVC   a3, [v1]
        LDMFD   sp!, {v1,pc}
@
