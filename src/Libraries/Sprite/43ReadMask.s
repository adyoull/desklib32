@
@       Title                  : Read mask pixel
@       System                 : Sprite Library
@       Version                : 1.0
@       Copyright              : (C) Ainsley Pereira
@       Date                   : Sun 27th February 94
@       Author                 : Ainsley M. Pereira
@
@       Function               : Read pixel from a mask
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
        
        .global Sprite_ReadMaskPixel
Sprite_ReadMaskPixel:
@
        STMFD     sp!, {v1-v2, lr}
	LDMIA     a3, {a4, v1}
        MOV       a3, a2
        MOV       a2, a1
        MOV       a1, #256
        ADD       a1, a1, #43
        SWI       OS_SpriteOp + XOS_Bit
	MOVVS     a1, #0
        MOVVC     a1, v2
        LDMFD     sp!, {v1-v2, pc}
@
