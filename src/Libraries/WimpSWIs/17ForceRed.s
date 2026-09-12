;
;       Title                  : Wimp Force Redraw
;       System                 : Wimp Library
;       Version                : 1.0
;       Copyright              : (C) John Winters
;       Date                   : 12th January, 1990
;       Author                 : John H. Winters
;
;       Function               : Forces a redraw
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
        STARTCODE Wimp_ForceRedraw
;
        STMFD   sp!, {v1, lr}
        LDMFD   a1, {a1, a2, a3, a4, v1}
        SWI     SWI_Wimp_ForceRedraw + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {v1, pc}
;
        END
