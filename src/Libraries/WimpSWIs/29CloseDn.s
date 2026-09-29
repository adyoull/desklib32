;
;       Title                  : Wimp Close Down.
;       System                 : Wimp Library
;       Version                : 1.0
;       Copyright              : (C) John Winters
;       Date                   : 12th January, 1990
;       Author                 : John H. Winters
;
;       Function               : Closes down a task.
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
        STARTCODE Wimp_CloseDown
;
        STMFD   sp!, {lr}
        LDR     a2, DATA
        SWI     SWI_Wimp_CloseDown + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {pc}
DATA
        DCB     "TASK"
;
        END
