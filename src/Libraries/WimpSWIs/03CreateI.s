@
@       Title                  : Wimp Create Icon.
@       System                 : Wimp Library
@       Version                : 1.0
@       Copyright              : (C) John Winters
@       Date                   : 12th January, 1990
@       Author                 : John H. Winters
@
@       Function               : Creates an Icon.
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
        
        .global Wimp_CreateIcon
Wimp_CreateIcon:
@
@        STMFD   sp!, {lr}
        MOV     ip, lr

        MOV     a4, a2
        MOV     a2, a1
        SWI     SWI_Wimp_CreateIcon + XOS_Bit
        STRVC   a1, [a4, #0]
        MOVVC   a1, #0

        MOV     pc, ip
@        LDMFD   sp!, {pc}
@
