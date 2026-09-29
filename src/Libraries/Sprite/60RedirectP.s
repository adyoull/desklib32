@
@       Title                  : Sprite 60N
@       System                 : Sprite Library
@       Version                : 1.0
@       Copyright              : (C) John Tytgat
@       Date                   : 29 Nov 2005
@       Author                 : John Tytgat
@
@       Function               : Issues an OS_SpriteOp 60 for a user sprite.
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
        
        .global Sprite_RedirectP
Sprite_RedirectP:
@
        STMFD   sp!, {a1, a2, a3, a4, lr}
        LDMFD   sp!, {a2, a3, a4, ip}
        MOV     a1, #512 + 60
        SWI     OS_SpriteOp + XOS_Bit
        STMVCIA ip, {a1, a2, a3, a4}
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
@
