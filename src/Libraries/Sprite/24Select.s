;
;       Title                  : Sprite 24
;       System                 : Sprite Library
;       Version                : 1.0
;       Copyright              : (C) Jason Howat
;       Date                   : 16th July, 1994
;       Author                 : Jason Howat
;
;       Function               : Issues an OS_SpriteOp 24.
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
        STARTCODE Sprite_Select
;
        STMFD   sp!, {lr}
        MOV     a4, a3
        MOV     a3, a2
        MOV     a2, a1
        MOV     a1, #24 + 256
        SWI     OS_SpriteOp + XOS_Bit
        LDMVSFD sp!, {pc}
        CMP     a4, #0
        STRNE   a3, [a4]
        MOV     a1, #0
        LDMFD   sp!, {pc}
;
        END
