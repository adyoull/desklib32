@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    Hourglass.s.Off
@   Author  Copyright © 1993 Jason Howat
@   Version 1.00 (24 Jul 1993)
@   Purpose Off

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@ void Hourglass_Off(void);

        
        .global Hourglass_Off
Hourglass_Off:
@
        STMFD   sp!, {lr}
        SWI     SWI_Hourglass_Off + XOS_Bit
        LDMFD   sp!, {pc}
@
