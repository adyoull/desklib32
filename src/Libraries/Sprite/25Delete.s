;
;       Title                  : Sprite 25N
;       System                 : Sprite Library
;       Version                : 1.0
;       Copyright              : (C) John Winters
;       Date                   : 9th February, 1992
;       Author                 : John H. Winters
;
;       Function               : Issues an OS_SpriteOp 25 for a user sprite.
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
        STARTCODE Sprite_Delete
;
        STMFD   sp!, {a1, a2, lr}
        LDMFD   sp!, {a2, a3}
        MOV     a1, #25
        ADD     a1, a1, #256
        SWI     OS_SpriteOp + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
;
        END
