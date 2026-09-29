@
@       Title                  : Wimp Set Pointer Shape.
@       System                 : Wimp Library
@       Version                : 1.0
@       Copyright              : (C) John Winters
@       Date                   : 12th January, 1990
@       Author                 : John H. Winters
@
@       Function               : Initialises the Wimp.
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
        
        .global Wimp_SetPointerShape
Wimp_SetPointerShape:
@
        STMFD   sp!, {v1, v2, lr}
        MOV     ip, a1
        LDMFD   ip!, {a1, a2, a3, a4, v1, v2}
        SWI     SWI_Wimp_SetPointerShape + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {v1, v2, pc}
@
