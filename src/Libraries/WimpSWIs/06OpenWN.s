@
@       Title                  : Wimp Open Window
@       System                 : Wimp Library
@       Version                : 1.0
@       Copyright              : (C) Peter Naulls
@       Date                   : 20th November, 2002
@       Author                 : John H. Winters
@
@       Function               : Opens a window in a parent
@
@
@       Modification history.
@
@       Version                : (Reflect in header IDENT)
@       Date                   :
@       Author                 :
@       Changes                :
@
@
@============================================================================
@
@  Include files.
@
@============================================================================
@
        .include     "RegDefs.h"
        .include     "SwiNos.h"
        .include     "Macros.h"
@
@============================================================================
@
@  Code.
@
@============================================================================
@
        
        .global Wimp_OpenWindowNest
Wimp_OpenWindowNest:
@
        STMFD   sp!, {v1, lr}

        MOV     a4, a2
        MOV     v1, a3
        MOV     a2, a1
        LDR     a3, task
        SWI     SWI_Wimp_OpenWindow + XOS_Bit
        MOVVC   a1, #0

        MOV     r2, #0  @ Prevent R2 retaining value
        LDMFD   sp!, {v1, lr}

task:
        .ascii    "TASK"
@
