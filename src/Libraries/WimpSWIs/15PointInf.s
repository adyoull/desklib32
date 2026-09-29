@
@       Title                  : Wimp Get Point Info.
@       System                 : Wimp Library
@       Version                : 1.0
@       Copyright              : (C) John Winters
@       Date                   : 12th January, 1990
@       Author                 : John H. Winters
@
@       Function               : Gets information about the pointer.
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
        
        .global Wimp_GetPointerInfo
Wimp_GetPointerInfo:
@
@        STMFD   sp!, {lr}
        MOV     ip, lr

        MOV     a2, a1
        SWI     SWI_Wimp_GetPointerInfo
        MOVVC   a1, #0

        MOV    pc, ip
@        LDMFD   sp!, {pc}
@
