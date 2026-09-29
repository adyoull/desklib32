@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    Filing.s.GBPB9
@   Author  Copyright © 1994, 1995 Sergio Monesi
@   Version 1.01 (6 Jun 1995)
@   Purpose Read directory entries (names only)
@   Mods    6 June 1995 - changed procedure name to a more
@                          DeskLib-compliant style

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@ os_error *Filing_ReadDirNames(char *dirname, void *buf,
@                               int *number, int *offset,
@                               int size, char *match);

        
        .global Filing_ReadDirNames
Filing_ReadDirNames:
@
        MOV     ip, sp
        STMFD   sp!, {r2-r6,lr}
        LDMIA   ip, {r5,r6}
        LDR     r4, [r3]
        LDR     r3, [r2]
        MOV     r2, r1
        MOV     r1, r0
        MOV     r0, #9
        SWI     SWI_OS_GBPB + XOS_Bit
        LDMVSFD sp!, {r2-r6,pc}
        MOV     r0,#0
        LDMFD   sp, {r5,r6}
        STR     r3, [r5]
        STR     r4, [r6]
        LDMFD   sp!, {r2-r6,pc}
@
