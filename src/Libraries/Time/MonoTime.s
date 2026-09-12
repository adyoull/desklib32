;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    Misc.MonoTime.s
;   Author:  Copyright © 1993 Jason Williams
;   Version: 1.00 (14 May 1993)
;   Purpose: Read time since startup in centiseconds (OS_ReadMonotonicTime)
;            (This is used in conjunction with Wimp_PollIdle)

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

        PREAMBLE
        STARTCODE Time_Monotonic
;
        MOV  ip, lr
        SWI  SWI_OS_ReadMonotonicTime + XOS_Bit
        MOV  pc, ip
;
        END
