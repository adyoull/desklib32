@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    Tinct.Plot.s
@   Author  Copyright © 2005 Peter Naulls
@   Version 1.00 (27 May 2005)

       .include     "RegDefs.h"
       .include     "SwiNos.h"
       .include     "Macros.h"

       .global Tinct_PlotScaledAlpha
Tinct_PlotScaledAlpha:

       MOV     ip, sp
       STMFD   sp!, {r4-r7,lr}
       MOV     r5, r3
       MOV     r4, r2
       MOV     r3, r1
       MOV     r2, r0
       LDMIA   ip, {r6, r7}
       SWI     SWI_Tinct_PlotScaledAlpha + XOS_Bit
       MOVVC   r0, #0
       LDMFD   sp!, {r4-r7,pc}

