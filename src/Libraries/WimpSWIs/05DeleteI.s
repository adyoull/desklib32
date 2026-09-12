;
;       Title                  : Wimp Delete Icon.
;       System                 : Wimp Library
;       Version                : 1.0
;       Copyright              : (C) John Winters
;       Date                   : 12th January, 1990
;       Author                 : John H. Winters
;
;       Function               : Deletes an icon.
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
        STARTCODE Wimp_DeleteIcon
;
        STMFD   sp!, {a1, a2, lr}
        MOV     a2, sp
        SWI     SWI_Wimp_DeleteIcon + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {a2, a3, pc}
;
        END
