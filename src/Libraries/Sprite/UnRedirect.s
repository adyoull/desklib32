@
@       Title                  : Sprite 60R
@       System                 : Sprite Library
@       Version                : 1.0
@       Copyright              : (C) John Winters
@       Date                   : 26th January, 1992
@       Author                 : John H. Winters
@
@       Function               : Issues an OS_SpriteOp 60/61.
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
        
        .global Sprite_UnRedirect
Sprite_UnRedirect:
@
        STMFD   sp!, {lr}
        LDMIA   a1, {a1, a2, a3, a4}
        SWI     OS_SpriteOp + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
@
