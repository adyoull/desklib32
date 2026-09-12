;
;       Title                  : Wimp Transfer Block
;       System                 : Wimp Library
;       Version                : 1.0
;       Copyright              : (C) John Winters
;       Date                   : 12th January, 1990
;       Author                 : John H. Winters
;
;       Function               : Transfer a block of memory from task
;                                to task.
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
        STARTCODE Wimp_TransferBlock
;
        MOV     ip, sp
        STMFD   sp!, {v1, lr}
        LDR     v1, [ip, #0]
        SWI     SWI_Wimp_TransferBlock + XOS_Bit
        MOVVC   a1, #0
        LDMFD   sp!, {v1, pc}
;
        END
