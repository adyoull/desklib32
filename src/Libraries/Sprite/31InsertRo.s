@
@       Title                  : Insert Row
@       System                 : Sprite Library
@       Version                : 1.0
@       Copyright              : (C) Ainsley Pereira
@       Date                   : Sun 27th February 94
@       Author                 : Ainsley M. Pereira
@
@       Function               : Insert row of pixels
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
        
        .global Sprite_InsertRow
Sprite_InsertRow:
@
        STMFD     sp!, {a1-a3, lr}
        LDMFD     sp!, {a2-a4}
        MOV       a1, #256
        ADD       a1, a1, #31
        SWI       OS_SpriteOp + XOS_Bit
        MOVVC     a1,#0
        LDMFD     sp!, {pc}
@
