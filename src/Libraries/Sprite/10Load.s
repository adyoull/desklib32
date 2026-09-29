@
@       Title                  : Sprite 10U
@       System                 : Sprite Library
@       Version                : 1.0
@       Copyright              : (C) John Winters
@       Date                   : 26th January, 1992
@       Author                 : John H. Winters
@
@       Function               : Issues an OS_SpriteOp 10.
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
        
        .global Sprite_Load
Sprite_Load:
@
        STMFD   sp!, {lr}
        MOV     a3, a2
        MOV     a2, a1
        MOV     a1, #256
        ADD     a1, a1, #10
        SWI     OS_SpriteOp + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
@
