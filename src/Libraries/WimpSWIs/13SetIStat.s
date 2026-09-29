;
;       Title                  : Wimp Set Icon State
;       System                 : Wimp Library
;       Version                : 1.0
;       Copyright              : (C) John Winters
;       Date                   : 12th January, 1990
;       Author                 : John H. Winters
;
;       Function               : Sets the state of an icon.
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
        STARTCODE Wimp_SetIconState
;
        STMFD   sp!, {a1-a4, lr}
        MOV     a2, sp
        SWI     SWI_Wimp_SetIconState + XOS_Bit
        MOVVC   a1, #0
        ADD     sp, sp, #16
        LDMFD   sp!, {pc}
;
        END
