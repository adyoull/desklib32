@
@       Title                  : Wimp Get Window State
@       System                 : Wimp Library
@       Version                : 1.0
@       Copyright              : (C) John Winters
@       Date                   : 12th January, 1990
@       Author                 : John H. Winters
@
@       Function               : Gets the state of a window.
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
        
        .global Wimp_GetWindowState
Wimp_GetWindowState:
@
        MOV     ip, lr

        STR     a1, [a2, #0]
        SWI     SWI_Wimp_GetWindowState + XOS_Bit
        MOVVC   a1, #0

        MOV     pc, ip
@
