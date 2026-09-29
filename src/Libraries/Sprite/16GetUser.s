@
@       Title                  : Sprite 16U
@       System                 : Sprite Library
@       Version                : 1.0
@       Copyright              : (C) John Winters
@       Date                   : 9th February, 1992
@       Author                 : John H. Winters
@
@       Function               : Issues an OS_SpriteOp 16.
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
        
        .global Sprite_Get
Sprite_Get:
@
        MOV     ip, sp
        STMFD   sp!, {a1-a4, v1-v4, lr}
        LDMFD   sp!, {a2, a3, a4, v1}
        LDMFD   ip!, {v2, v3, v4}
        MOV     a1, #256 + 16
        SWI     OS_SpriteOp + XOS_Bit
        LDRVC   ip, [ip, #0]
        STRVC   a3, [ip, #0]
        MOVVC   a1, #0
        LDMFD   sp!, {v1-v4, pc}
@
