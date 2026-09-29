@
@       Title                  : Wimp Base Of Sprites.
@       System                 : Wimp Library
@       Version                : 1.0
@       Copyright              : (C) John Winters
@       Date                   : 12th January, 1990
@       Author                 : John H. Winters
@
@       Function               : Gets pointers to the sprite areas.
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
        
        .global Wimp_BaseOfSprites
Wimp_BaseOfSprites:
@
@        STMFD   sp!, {lr}
        MOV     ip, lr

        MOV     a3, a1
        MOV     a4, a2
        SWI     SWI_Wimp_BaseOfSprites + XOS_Bit
        STRVC   a1, [a3, #0]
        STRVC   a2, [a4, #0]
        MOVVC   a1, #0

        MOV     pc, ip
@        LDMFD   sp!, {pc}
@
