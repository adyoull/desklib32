@
@       Title                  : Wimp Set Caret Position.
@       System                 : Wimp Library
@       Version                : 1.0
@       Copyright              : (C) John Winters
@       Date                   : 12th January, 1990
@       Author                 : John H. Winters
@
@       Function               : Sets the position of the caret.
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
        
        .global Wimp_SetCaretPosition
Wimp_SetCaretPosition:
@
        STMFD   sp!, {v1, v2, lr}
        LDMFD   a1, {a1, a2, a3, a4, v1, v2}
        SWI     SWI_Wimp_SetCaretPosition + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {v1, v2, pc}
@
