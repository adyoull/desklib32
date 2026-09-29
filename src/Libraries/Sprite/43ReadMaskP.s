@
@       Title                  : Read mask pixel
@       System                 : Sprite Library
@       Version                : 1.0
@       Copyright              : (C) John Tytgat
@       Date                   : 29 Nov 2005
@       Author                 : John Tytgat
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
        
        .global Sprite_ReadMaskPixelP
Sprite_ReadMaskPixelP:
@
        STMFD     sp!, {v1-v2, lr}
	LDMIA     a3, {a4, v1}
        MOV       a3, a2
        MOV       a2, a1
        MOV       a1, #512
        ADD       a1, a1, #43
        SWI       OS_SpriteOp + XOS_Bit
	MOVVS     a1, #0
        MOVVC     a1, v2
        LDMFD     sp!, {v1-v2, pc}
@
