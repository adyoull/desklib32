;
;       Title                  : Wimp SpriteOp Full.
;       System                 : Wimp Library
;       Version                : 1.0
;       Copyright              : (C) John Winters
;       Date                   : 12th January, 1990
;       Author                 : John H. Winters
;
;       Function               : Silly routine to issue a SpriteOp call.
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
        STARTCODE Wimp_SpriteOp
;
        STMFD   sp!, {v1-v6, lr}
        MOV     ip, a1
        LDMIA   ip, {a1-v6}
        SWI     SWI_Wimp_SpriteOp + XOS_Bit
        STMIA   ip, {a1-v6}
        MOVVC   a1, #0
        LDMFD   sp!, {v1-v6, pc}
;
        END
