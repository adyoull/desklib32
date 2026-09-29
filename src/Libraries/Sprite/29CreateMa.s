@
@       Title                  : Create mask
@       System                 : Sprite Library
@       Version                : 1.0
@       Copyright              : (C) Ainsley Pereira
@       Date                   : Sun 27th February 94
@       Author                 : Ainsley M. Pereira
@
@       Function               : Creates a mask for a sprite
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
        
        .global Sprite_CreateMask
Sprite_CreateMask:
@
        STMFD     sp!, {a1-a2, lr}
        LDMFD     sp!, {a2-a3}
        MOV       a1, #256
        ADD       a1, a1, #29
        SWI       OS_SpriteOp + XOS_Bit
        MOVVC     a1, #0
        LDMFD     sp!, {pc}
@
