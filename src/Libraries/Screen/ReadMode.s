@   ####             #    #     # #
@   #   #            #    #       #          The FreeWare C library for
@   #   #  ##   ###  #  # #     # ###             RISC OS machines
@   #   # #  # #     # #  #     # #  #   ___________________________________
@   #   # ####  ###  ##   #     # #  #
@   #   # #        # # #  #     # #  #    Please refer to the accompanying
@   ####   ### ####  #  # ##### # ###    documentation for conditions of use
@   ________________________________________________________________________
@
@   File:    Screen/ReadMode.s
@   Author:  Copyright © 1994 Jason Howat
@   Version: 1.00 (24 Jun 1994)
@            1.01 (10 Oct 2007) Moved to Screen module
@   Purpose: Generic veneer to the OS_ReadModeVariable SWI.

        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"

@ os_error *Screen_ReadModeVariable(int mode, mode_variable variable, int *value);


        .global Screen_ReadModeVariable
Screen_ReadModeVariable:
@
        MOV     ip, sp
        STMFD   sp!, {lr}
        MOV     r3, r2
        SWI     SWI_OS_ReadModeVariable + XOS_Bit
        MOVVC   r0, #0
        ADRCS   r0, error
        CMP     r3, #0
        STRNE   r2, [r3]
        LDMFD   sp!, {pc}
@
error:  .word     0
        .asciz    "Invalid mode or variable number"
@
