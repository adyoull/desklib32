@
@       Title                  : Sprite 34N
@       System                 : Sprite Library
@       Version                : 1.0
@       Copyright              : (C) John Tytgat
@       Date                   : 29 Nov 2005
@       Author                 : John Tytgat
@
@       Function               : Issues an OS_SpriteOp 34 for a user sprite.
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
        
        .global Sprite_PlotP
Sprite_PlotP:
@
        MOV     ip, sp
        STMFD   sp!, {a1-a4, v1-v2, lr}
        LDMFD   sp!, {a2, a3, a4, v1}
        LDR     v2, [ip, #0]
        MOV     a1, #512
        ADD     a1, a1, #34
        SWI     OS_SpriteOp + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {v1-v2, pc}
@
