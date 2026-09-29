@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    Misc.MonoTime.s
@   Author  Copyright © 1993 Jason Williams
@   Version 1.00 (14 May 1993)
@   Purpose Read time since startup in centiseconds (OS_ReadMonotonicTime)
@            (This is used in conjunction with Wimp_PollIdle)

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

        
        .global Time_Monotonic
Time_Monotonic:
@
        MOV  ip, lr
        SWI  SWI_OS_ReadMonotonicTime + XOS_Bit
        MOV  pc, ip
@
