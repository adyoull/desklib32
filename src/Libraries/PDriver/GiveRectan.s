@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    PDriver.GiveRectan.s
@   Author  Copyright © 1993 Jason Howat
@   Version 1.00 (08 Jul 1993)
@   Purpose Specify a rectangle to be printed.

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@ os_error *PDriver_GiveRectangle(int rectangle_id, wimp_rect *rectangle,
@                                 print_transformation *matrix,
@                                 wimp_point *position, int background_colour);

        
        .global PDriver_GiveRectangle
PDriver_GiveRectangle:
@
        MOV     ip, sp
        STMFD   sp!, {v1,lr}
        LDR     v1, [ip, #0]
        SWI     SWI_PDriver_GiveRectangle + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {v1,pc}
@
