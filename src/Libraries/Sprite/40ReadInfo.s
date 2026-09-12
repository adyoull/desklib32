;
;       Title                  : Sprite 40N
;       System                 : Sprite Library
;       Version                : 1.0
;       Copyright              : (C) John Winters
;       Date                   : 26th January, 1992
;       Author                 : John H. Winters
;
;       Function               : Issues an OS_SpriteOp 40 for a user sprite.
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
        STARTCODE Sprite_ReadInfo
;
        STMFD   sp!, {a3, v1-v3, lr}
        MOV     a3, a2
        MOV     a2, a1
        MOV     a1, #256 + 40
        SWI     OS_SpriteOp + XOS_Bit
        LDMFD   sp!, {a2}
        STMVCIA a2, {a4, v1-v3}
        MOVVC   a1, #0
        LDMFD   sp!, {v1-v3, pc}
;
        END
