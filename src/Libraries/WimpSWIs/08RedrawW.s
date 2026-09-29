@
@       Title                  : Wimp Redraw Window.
@       System                 : Wimp Library
@       Version                : 1.0
@       Copyright              : (C) John Winters
@       Date                   : 12th January, 1990
@       Author                 : John H. Winters
@
@       Function               : Redraw a window.
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
        
        .global Wimp_RedrawWindow
Wimp_RedrawWindow:
@
@        STMFD   sp!, {lr}
        MOV     ip, lr

        MOV     a4, a2
        MOV     a2, a1
        SWI     SWI_Wimp_RedrawWindow + XOS_Bit
        STRVC   a1, [a4, #0]
        MOVVC   a1, #0

        MOV    pc, ip
@        LDMFD   sp!, {pc}
@
