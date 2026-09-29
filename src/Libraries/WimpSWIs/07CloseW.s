@
@       Title                  : Wimp Close Window.
@       System                 : Wimp Library
@       Version                : 1.0
@       Copyright              : (C) John Winters
@       Date                   : 12th January, 1990
@       Author                 : John H. Winters
@
@       Function               : Closes a window.
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
        
        .global Wimp_CloseWindow
Wimp_CloseWindow:
@
        STMFD   sp!, {a1, lr}
        MOV     a2, sp
        SWI     SWI_Wimp_CloseWindow + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {a2, pc}
@
