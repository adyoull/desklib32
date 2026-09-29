@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    Hourglass.s.Start
@   Author  Copyright © 1993 Jason Howat
@   Version 1.00 (24 Jul 1993)
@   Purpose Start the hourglass after a delay

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@ void Hourglass_Start(int startdelay);

        
        .global Hourglass_Start
Hourglass_Start:
@
        STMFD   sp!, {lr}
        SWI     SWI_Hourglass_Start + XOS_Bit
        LDMFD   sp!, {pc}
@
