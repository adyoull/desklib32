;
;       Title                  : Sprite 15U
;       System                 : Sprite Library
;       Version                : 1.0
;       Copyright              : (C) John Winters
;       Date                   : 26th January, 1992
;       Author                 : John H. Winters
;
;       Function               : Issues an OS_SpriteOp 15.
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
        STARTCODE Sprite_Create
;
        MOV     ip, sp
        STMFD   sp!, {a1-a4, v1-v3, lr}
        LDMIA   ip, {v2, v3}
        LDMFD   sp!, {a2, a3, a4, v1}
        MOV     a1, #256
        ADD     a1, a1, #15
        SWI     OS_SpriteOp + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {v1-v3, pc}
;
        END
