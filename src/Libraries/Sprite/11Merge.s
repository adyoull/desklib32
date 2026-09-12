;
;       Title                  : Sprite 11U
;       System                 : Sprite Library
;       Version                : 1.0
;       Copyright              : (C) John Winters
;       Date                   : 26th January, 1992
;       Author                 : John H. Winters
;
;       Function               : Issues an OS_SpriteOp 11.
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
        STARTCODE Sprite_Merge
;
        STMFD   sp!, {lr}
        MOV     a3, a2
        MOV     a2, a1
        MOV     a1, #256
        ADD     a1, a1, #11
        SWI     OS_SpriteOp + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
;
        END
