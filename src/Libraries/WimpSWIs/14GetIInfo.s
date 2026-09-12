;
;       Title                  : Wimp Get Icon Info
;       System                 : Wimp Library
;       Version                : 1.0
;       Copyright              : (C) John Winters
;       Date                   : 12th January, 1990
;       Author                 : John H. Winters
;
;       Function               : Gets information about an Icon.
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
        STARTCODE Wimp_GetIconState
;
        STMFD   sp!, {v1, v2, v3, v4, lr}
;
;  Need a 40 byte block for general storage.
;
        SUB     sp, sp, #40
        STR     a1, [sp, #0]
        STR     a2, [sp, #4]
        MOV     a2, sp
        SWI     SWI_Wimp_GetIconState + XOS_Bit
        ADD     sp, sp, #8
;
;  Copy 32 bytes, i.e. 8 registers full into user space.
;
        LDMFD   sp!, {a1, a2, a4, v1, v2, v3, v4, ip}
        STMVCIA a3, {a1, a2, a4, v1, v2, v3, v4, ip}
        MOVVC   a1, #0
        LDMFD   sp!, {v1, v2, v3, v4, pc}
;
        END
