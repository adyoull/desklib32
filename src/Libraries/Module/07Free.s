@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    Module.s.07Free
@   Author  Copyright © 1993 Jason Howat
@   Version 1.00 (23 Nov 1993)
@   Purpose Deallocate a block of the RMA.

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@ os_error *Module_Free(void *block);


        
        .global Module_Free
Module_Free:
@
        STMFD   sp!, {lr}
        MOV     a3, a1
        MOV     a1, #7
        SWI     SWI_OS_Module + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
@
