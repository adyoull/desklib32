@
@       Title                  : Delete Row
@       System                 : Sprite Library
@       Version                : 1.0
@       Copyright              : (C) John Tytgat
@       Date                   : 29 Nov 2005
@       Author                 : John Tytgat
@
@       Function               : Delete row of pixels
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
        
        .global Sprite_DeleteRowP
Sprite_DeleteRowP:
@
        STMFD     sp!, {a1-a3, lr}
        LDMFD     sp!, {a2-a4}
        MOV       a1, #512 + 32
        SWI       OS_SpriteOp + XOS_Bit
        MOVVC     a1,#0
        LDMFD     sp!, {pc}
@
