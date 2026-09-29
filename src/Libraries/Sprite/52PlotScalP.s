@
@       Title                  : Plot Scaled
@       System                 : Sprite Library
@       Version                : 1.0
@       Copyright              : (C) John Tytgat
@       Date                   : 29 Nov 2005
@       Author                 : John Tytgat
@
@       Function               : Plots sprite scaled
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
        
        .global Sprite_PlotScaledP
Sprite_PlotScaledP:
@
        MOV       ip, sp
        STMFD     sp!, {v1-v4, lr}
        MOV       v2, a4
        LDMFD     ip, {v3, v4}
        LDMIA     a3, {a4, v1}
        MOV       a3, a2
        MOV       a2, a1
        MOV       a1, #512 + 52
        SWI       OS_SpriteOp + XOS_Bit
        MOVVC     a1, #0
        LDMFD     sp!, {v1-v4, pc}
@
