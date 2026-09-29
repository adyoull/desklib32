@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    PDriver.CheckFeatu.s
@   Author  Copyright © 1993 Jason Howat
@   Version 1.00 (06 Jul 1993)
@   Purpose Check the features of a printer and return an error
@            if appropriate.

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@ os_error *PDriver_CheckFeatures(print_features mask, print_features value);

        
        .global PDriver_CheckFeatures
PDriver_CheckFeatures:
@
        STMFD   sp!, {lr}
        SWI     SWI_PDriver_CheckFeatures + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
@
