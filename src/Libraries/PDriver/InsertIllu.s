@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    PDriver.InsertIllu.s
@   Author  Copyright © 1993 Jason Howat
@   Version 1.00 (07 Jul 1993)
@   Purpose Inserts a file containing an illustration into the
@            current job's output.

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@ os_error *PDriver_InsertIllustration(int illustration_handle, void *drawpath,
@                                      wimp_point *bottom_left,
@                                      wimp_point *bottom_right,
@                                      wimp_point *top_left);

        
        .global PDriver_InsertIllustration
PDriver_InsertIllustration:
@
        MOV     ip, sp
        STMFD   sp!, {v1,v2,v3,v4,lr}
        LDR     v1, [ip, #0]
        LDMIA   v1, {r6,r7}
        LDMIA   a4, {r4,r5}
        LDMIA   a3, {r2,r3}
        SWI     SWI_PDriver_InsertIllustration + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {v1,v2,v3,v4,pc}
@
