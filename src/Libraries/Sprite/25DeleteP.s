@
@       Title                  : Sprite 25N
@       System                 : Sprite Library
@       Version                : 1.0
@       Copyright              : (C) John Tytgat
@       Date                   : 29 Nov 2005
@       Author                 : John Tytgat
@
@       Function               : Issues an OS_SpriteOp 25 for a user sprite.
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
        
        .global Sprite_DeleteP
Sprite_DeleteP:
@
        STMFD   sp!, {lr}
	MOV	a3, a2
	MOV	a2, a1
        MOV     a1, #25
        ADD     a1, a1, #512
        SWI     OS_SpriteOp + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
@
