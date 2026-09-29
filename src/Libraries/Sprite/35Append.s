@
@       Title                  : Append Sprite
@       System                 : Sprite Library
@       Version                : 1.0
@       Copyright              : (C) Ainsley Pereira
@       Date                   : Sun 27th February 94
@       Author                 : Ainsley M. Pereira
@
@       Function               : Appends sprites
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
        
        .global Sprite_Append
Sprite_Append:
@
        STMFD     sp!, {a1-a4, v1, lr}
        LDMFD     sp!, {a2-a4, v1}
        MOV       a1, #256
        ADD	  a1, a1, #35
        SWI       OS_SpriteOp + XOS_Bit
        MOVVC     a1, #0
        LDMFD     sp!, {v1, pc}
@
