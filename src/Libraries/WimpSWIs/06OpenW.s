@
@       Title                  : Wimp Open Window
@       System                 : Wimp Library
@       Version                : 1.0
@       Copyright              : (C) John Winters
@       Date                   : 12th January, 1990
@       Author                 : John H. Winters
@
@       Function               : Opens a window.
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
        
        .global Wimp_OpenWindow
Wimp_OpenWindow:

        MOV     ip, lr

        MOV     a2, a1
        MOV     a3, #0
        SWI     SWI_Wimp_OpenWindow + XOS_Bit
        MOVVC   a1, #0

        MOV     pc, ip

