;
;       Title                  : Sprite 60N
;       System                 : Sprite Library
;       Version                : 1.0
;       Copyright              : (C) John Winters
;       Date                   : 26th January, 1992
;       Author                 : John H. Winters
;
;       Function               : Issues an OS_SpriteOp 60 for a user sprite.
;
;
;       Modification history.
;
;       Version                : (Reflect in header IDENT)
;       Date                   :
;       Author                 :
;       Changes                :
;
;
;============================================================================
;
;  Include files.
;
;============================================================================
;
        GET     RegDefs.h
        GET     SwiNos.h
        GET     Macros.h
;
;============================================================================
;
;  Code.
;
;============================================================================
;
        PREAMBLE
        STARTCODE Sprite_Redirect
;
        STMFD   sp!, {a1, a2, a3, a4, lr}
        LDMFD   sp!, {a2, a3, a4, ip}
        MOV     a1, #256 + 60
        SWI     OS_SpriteOp + XOS_Bit
        STMVCIA ip, {a1, a2, a3, a4}
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
;
        END
