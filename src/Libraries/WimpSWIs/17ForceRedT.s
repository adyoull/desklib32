;
;       Title                  : Wimp Force Redraw Title
;       System                 : Wimp Library
;       Version                : 1.0
;       Copyright              : (C) Peter Naulls
;       Date                   : 21st November 2002
;       Author                 : Peter Naulls
;
;       Function               : Forces a redraw of the title
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
        STARTCODE Wimp_RedrawTitle
;
        LDR     a2, |task|
        MOV     a3, #3        

        SWI     SWI_Wimp_ForceRedraw + XOS_Bit
        MOVVC   a1, #0

        MOV     a2, #0  ; Prevent R2 retaining value
        MOV     pc, lr
|task|
        DCB    "TASK"
;
        END
