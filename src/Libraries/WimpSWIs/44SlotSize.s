@
@       Title                  : Wimp Slot Size.
@       System                 : Wimp Library
@       Version                : 1.0
@       Copyright              : (C) John Winters
@       Date                   : 12th January, 1990
@       Author                 : John H. Winters
@
@       Function               : Changes the slot size of the process.
@                                N.B.  This version does not cater for ASD.
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
        
        .global Wimp_SlotSize
Wimp_SlotSize:
@
@  There is a nasty bug in Risc-OS.  R4 (v1) is not preserved during calls
@  to Wimp_SlotSize.  Hence can't use it as temporary workspace.
@
        STMFD   sp!, {v1, v2, v3, lr}
        MOV     a4, a1
        MOV     v2, a2
        MOV     v3, a3
        LDR     a1, [a4, #0]
        LDR     a2, [v2, #0]
        MOV     a3, #0
        SWI     SWI_Wimp_SlotSize + XOS_Bit
        STRVC   a1, [a4, #0]
        STRVC   a2, [v2, #0]
        STRVC   a3, [v3, #0]
        MOVVC   a1, #0
        LDMFD   sp!, {v1, v2, v3, pc}
@
