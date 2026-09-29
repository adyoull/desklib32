@
@       Title                  : Sprite 26N
@       System                 : Sprite Library
@       Version                : 1.0
@       Copyright              : (C) John Winters
@       Date                   : 9th February, 1992
@       Author                 : John H. Winters
@
@       Function               : Issues an OS_SpriteOp 26 for a user sprite.
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
        
        .global Sprite_Rename
Sprite_Rename:
@
        STMFD   sp!, {a1, a2, a3, lr}
        LDMFD   sp!, {a2, a3, a4}
        MOV     a1, #26
        ADD     a1, a1, #256
        SWI     OS_SpriteOp + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
@
