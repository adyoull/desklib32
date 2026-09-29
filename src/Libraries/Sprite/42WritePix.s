@
@       Title                  : Write pixel
@       System                 : Sprite Library
@       Version                : 1.0
@       Copyright              : (C) Ainsley Pereira
@       Date                   : Sun 27th February 94
@       Author                 : Ainsley M. Pereira
@
@       Function               : Write pixel to a sprite
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
        
        .global Sprite_WritePixel
Sprite_WritePixel:
@
        STMFD     sp!, {v1-v3, lr}
        LDR       v3, [sp, #4*4]
        MOV       v2, a4
	LDMIA     a3, {a4, v1}
        MOV       a3, a2
        MOV       a2, a1
        MOV	  a1, #256
        ADD	  a1, a1, #42
        SWI       OS_SpriteOp + XOS_Bit
        MOVVC     a1, #0
        LDMFD     sp!, {v1-v3, pc}
@
