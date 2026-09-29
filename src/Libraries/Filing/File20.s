@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File    Filing.s.File20
@   Author  Copyright © 1994, 1995 Sergio Monesi
@   Version 1.01 (6 Jun 1995)
@   Purpose Read catalogue information
@   Mods    6 June 1995 - changed procedure name to a more
@                          DeskLib-compliant style

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@ os_error *Filing_ReadCatalogue(char *filename, filing_objtype *objtype,
@                                int *loadaddr, int *execaddr, int *length,
@                                int *attrib, int *filetype);

        
        .global Filing_ReadCatalogue
Filing_ReadCatalogue:
@
        STMFD   sp!, {r1-r6,lr}
        MOV     r1, r0
        MOV     r0, #20
        SWI     SWI_OS_File + XOS_Bit
        LDMVSFD sp!, {r1-r6,pc}
        LDR     lr, [sp]
        TEQ     lr, #0
        STRNE   r0, [lr]
        LDR     lr, [sp, #4]
        TEQ     lr ,#0
        STRNE   r2, [lr]
        LDR     lr, [sp, #8]
        TEQ     lr, #0
        STRNE   r3, [lr]
        LDR     lr, [sp ,#28]
        TEQ     lr, #0
        STRNE   r4, [lr]
        LDR     lr, [sp ,#32]
        TEQ     lr, #0
        STRNE   r5, [lr]
        LDR     lr, [sp ,#36]
        TEQ     lr, #0
        STRNE   r6, [lr]
        MOV     r0,#0
        LDMFD   sp!, {r1-r6,pc}
@
