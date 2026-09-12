;   ####             #    #     # #
;   #   #            #    #       #          The FreeWare C library for
;   #   #  ##   ###  #  # #     # ###             RISC OS machines
;   #   # #  # #     # #  #     # #  #   ___________________________________
;   #   # ####  ###  ##   #     # #  #
;   #   # #        # # #  #     # #  #    Please refer to the accompanying
;   ####   ### ####  #  # ##### # ###    documentation for conditions of use
;   ________________________________________________________________________
;
;   File:    KernelSWIs.s.ReadModeVa
;   Author:  Copyright © 1994 Jason Howat
;   Version: 1.00 (24 Jun 1994)
;   Purpose: Generic veneer to the OS_ReadModeVariable SWI.

        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h

; os_error *OS_ReadModeVariable(int mode, mode_variable variable, int *value);

        PREAMBLE
        STARTCODE OS_ReadModeVariable
;
        MOV     ip, sp
        STMFD   sp!, {lr}
        MOV     r3, r2
        SWI     SWI_OS_ReadModeVariable + XOS_Bit
        MOVVC   r0, #0
        ADRCS   r0, error
        CMP     r3, #0
        STRNE   r2, [r3]
        LDMFD   sp!, {pc}
;
error   DCD     0
        =       "Invalid mode or variable number",0
;
        END
