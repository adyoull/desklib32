@
@       Title                  : Plot grey
@       System                 : Sprite Library
@       Version                : 1.0
@       Copyright              : (C) John Tytgat
@       Date                   : 29 Nov 2005
@       Author                 : John Tytgat
@
@       Function               : Plots sprite greyscaled/antialiased
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
        
        .global Sprite_PlotGreyP
Sprite_PlotGreyP:
@
        STMFD     sp!, {v1-v4, lr}
        LDR       v4, [sp, #4*5]
        MOV       v3, a4
        MOV       v2, #0
	LDMIA     a3, {a4, v1}
        MOV       a3, a2
        MOV       a2, a1
        MOV       a1, #512
        ADD       a1, a1, #53
        SWI       OS_SpriteOp + XOS_Bit
        MOVVC     a1, #0
        LDMFD     sp!, {v1-v4, pc}
@
